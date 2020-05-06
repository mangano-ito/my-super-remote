import os
from time import sleep

from .service.ir_serial import IrSerial

def main():
    ir_port = os.environ.get('IR_SERIAL_PORT', '/dev/ttyUSB0')
    ir = IrSerial.from_port(ir_port)
    print('start capturing')
    ir.capture()
    print('capture done.')
    sleep(1)
    ir.print_captured()
    ir.emit_captured()

if __name__ == '__main__':
    main()
