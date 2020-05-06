from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware

import os
from time import sleep
from typing import List

from .service.ir_serial import IrSerial
from .repository.presets import Presets

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_credentials=True,
    allow_origins=['*'],
    allow_methods=['*'],
    allow_headers=['*'],
)

ir_port = os.environ.get('IR_SERIAL_PORT', '/dev/ttyUSB0')
ir = IrSerial.from_port(ir_port)

@app.get('/preset')
def all_presets() -> List:
    """
    list all the known presets

    :returns: all the presets
    """

    return list(Presets.list_all().values())

@app.post('/preset/{key}')
def send_preset(key: str):
    """
    send preset signal sequence

    :param key: key of preset
    :returns: response
    """

    try:
        preset = Presets.load(key)
    except KeyError:
        raise HTTPException(status_code=400, detail='key not found')

    try:
        ir.send_sequence(preset)
        sleep(0.5)
        ir.emit_captured()
    except Exception as e:
        print(e)
        raise HTTPException(status_code=500, detail='Ir communication error: {}'.format(e))

    return {'result': 'success'}
