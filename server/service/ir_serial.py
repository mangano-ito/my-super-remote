from time import sleep
from typing import List

from serial import Serial

type Sequence = List[int]

class IrSerial:
    """
    Ir-Serial Controller
    """

    OPERATOR_RECORD_START = 'r'.encode('ascii')
    OPERATOR_RECORD_END = 'e'.encode('ascii')
    OPERATOR_LOAD = 'l'.encode('ascii')
    OPERATOR_EMIT_CAPTURED = 'e'.encode('ascii')
    OPERATOR_PRINT_CAPTURED = 'p'.encode('ascii')

    def __init__(self, serial_connection: Serial) -> self:
        """
        :param serial_connection: PySerial serial connection
        """

        self.conn = serial_connection

    @classmethod
    def from_port(cls, port = '/dev/ttyUSB0') -> IrSerial:
        """
        create instance from device port

        :returns: created instance
        """

        conn = Serial(baudrate=9600, dsrdtr=None, xonxoff=None)
        conn.port = port
        conn.open()
        conn.flushInput()
        sleep(2)

        return IrSerial(conn)

    def capture(self):
        """
        start capturing signals and wait for complete
        """

        self._write_bytes(self.OPERATOR_RECORD_START)
        sleep(2)
        self._write_bytes(self.OPERATOR_RECORD_END)

    def send_sequence(self, sequence: Sequence):
        """
        send preset signal sequence

        :param sequence: sequence to send
        """

        self._write_bytes(self.OPERATOR_LOAD)
        self._write_sequence(sequence)

    def emit_captured(self):
        """
        emit captured signal sequence
        """

        self._write_bytes(self.OPERATOR_EMIT_CAPTURED)

    def print_captured(self):
        """
        prettyprint captured signal sequence
        """

        self._write_bytes(self.OPERATOR_PRINT_CAPTURED)
        while True:
            line = self._read_line()
            print(line)
            if line.startswith('> end of data'):
                return

    def _write_sequence(self, sequence: Sequence):
        """
        write signal sequence data
        """

        # send length first
        self._write_int(len(sequence))
        for signal in sequence:
            self._write_int(signal)

    def _write_int(self, signal: int):
        """
        just write int in multibyte manner + Big Endian
        """

        self._write_bytes(signal.to_bytes(2, 'big', signed=True))

    def _write_bytes(self, signal_bytes: bytes):
        """
        write byte array
        """

        self.conn.write(signal_bytes)

    def _read_line(self) -> str:
        """
        just read a line

        :returns: a line from serial
        """

        return self.conn.readline().decode('utf-8')

