#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define IR_INPUT_PIN (4)
#define IR_OUTPUT_PIN (3)
#define IR_OUTPUT_PIN_BIT (1 << IR_OUTPUT_PIN)
#define IR_OUTPUT_ON() (PORTD |= IR_OUTPUT_PIN_BIT)
#define IR_OUTPUT_OFF() (PORTD &= ~IR_OUTPUT_PIN_BIT)
#define IR_OUTPUT_ON_DURATION (8)
#define IR_OUTPUT_OFF_DURATION (18)

#define OPERATOR_RECORD ('r')
#define OPERATOR_RECORD_FINISH ('e')
#define OPERATOR_SAVE ('s')
#define OPERATOR_LOAD ('l')
#define OPERATOR_SEND ('e')
#define OPERATOR_PRINT ('p')

#define RESPONSE_END_OF_DATA ("> end of data")

#define SEQUENCE_BUFFER_SIZE_MAX (512)

typedef uint16_t WORD;
typedef uint8_t BYTE;
typedef bool BIT;
typedef long duration_t;
typedef char operator_t;

/**
 * Sequence Data Structure
 */
typedef struct
{
    /** signal sequence buffer */
    WORD buffer[SEQUENCE_BUFFER_SIZE_MAX] = {};
    /** signal sequence length */
    size_t length = 0;
} Sequence;


/** globally allocated sequence object */
Sequence sequence;

/**
 * reset sequence buffer
 */
void sequence_reset()
{
    sequence.length = 0;
    memset(sequence.buffer, 0, sizeof(WORD) * SEQUENCE_BUFFER_SIZE_MAX);
}

/**
 * add sequence data
 */
void sequence_add_data(WORD element)
{
    // XXX: what if sequence_size exceeds 512?
    sequence.buffer[sequence.length++] = element;
}

/**
 * record signal through Ir input
 */
void record_impl()
{
    static duration_t lastTime = micros();
    static BIT last = 1;
    // we get signal bit flipped (1 sent -> 0 received)
    // the very first bit is expected to be HIGH(1)
    // then, the actual sequence would be: [1, 0, 1, ...]
    // so we just record each signal's duration.

    BIT in = digitalRead(IR_INPUT_PIN);
    // signal remains unchanged 
    if (last == in) return;

    // XXX: WORD (aka uint16_t) possibly overflows for duration_t (aka int32_t)
    duration_t now = micros();
    WORD diff = (WORD)(now - lastTime);
    sequence_add_data(diff);

    lastTime = now;
    last = in;
}

/**
 * record sequence through Ir input
 */
void record()
{
    sequence_reset();
    do {
        record_impl();
    } while (Serial.read() != OPERATOR_RECORD_FINISH);
}

/**
 * emit signal
 *
 * @param data signal bit to emit (low: 0, high: !0)
 */
inline void emit_signal(BIT data)
{
    // no need to emit while the data bit is low (0)
    if (!data) return;

    // we assign value on output port directly
    // so that the operations can be fast enough to keep 38kHz
    IR_OUTPUT_ON();
    delayMicroseconds(IR_OUTPUT_ON_DURATION);
    IR_OUTPUT_OFF();
    delayMicroseconds(IR_OUTPUT_OFF_DURATION);
}

/**
 * emit signal in given duration
 * 
 * @param data signal bit to emit (low: 0, high: !0)
 * @param duration duration of signal
 */
void emit_signal_during(BIT data, duration_t duration)
{
    duration_t expire_time = micros() + duration;
    do {
        // we transcode data at 38kHz rate
        emit_signal(data);
    } while (micros() < expire_time);
}

/**
 * send recorded signal sequence
 */
void send_sequence()
{
    for (size_t i = 0; i < sequence.length; ++i) {
        // the first bit (i == 0) should be 1 (HIGH)
        // so the following bits should be flipped repeatedly
        emit_signal_during(i % 2, sequence.buffer[i]);
    }
    emit_signal(0);
}

/**
 * save signal sequence to serial output
 */
void save()
{
    Serial.write(sequence.length);
    for (size_t i = 0; i < sequence.length; ++i) {
        Serial.write(sequence.buffer[i]);
    }
    Serial.println(RESPONSE_END_OF_DATA);
}

/**
 * blocking-read data from serial input as WORD (big endian)
 *
 * @return read data
 */
inline WORD read_word()
{
    while (!Serial.available());
    BYTE high = (BYTE) Serial.read();
    while (!Serial.available());
    BYTE low = (BYTE) Serial.read();
    WORD result = (WORD) ((high << 8) | low);

    return result;
}

/**
 * load signal sequence from serial input
 */
void load()
{
    sequence.length = read_word();
    for (size_t i = 0; i < sequence.length; ++i) {
        sequence.buffer[i] = read_word();
    }
}

/**
 * prettyprint recorded sequence
 */
void print()
{
    Serial.print("> recorded sequence = [");
    for (size_t i = 0; i < sequence.length; ++i) {
        Serial.print(sequence.buffer[i], DEC);
        if (i < sequence.length - 1)
            Serial.print(", ");
    }
    Serial.println("]");
    Serial.println(RESPONSE_END_OF_DATA);
}

/**
 * invoke given operation
 * @param op operator
 */
void invoke(operator_t op)
{
    switch (op) {
        case OPERATOR_RECORD:
            return record();
        case OPERATOR_SAVE:
            return save();
        case OPERATOR_LOAD:
            return load();
        case OPERATOR_SEND:
            return send_sequence();
        case OPERATOR_PRINT:
            return print();
    }
}

/**
 * main event loop called by Arduino bootstrap code
 */
void loop()
{
    if (!Serial.available()) return;

    operator_t op = Serial.read();
    invoke(op);
}

/**
 * initialization called by Arduino bootstrap code
 */
void setup()
{
    pinMode(IR_OUTPUT_PIN, OUTPUT);
    pinMode(IR_INPUT_PIN, INPUT);
    Serial.begin(9600);
    sequence_reset();
}
