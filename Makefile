ARDUINO_PORT=/dev/ttyUSB1
ARDUINO_FQDN=arduino:avr:diecimila:cpu=atmega328

.PHONY: arduino
arduino: arduino-upload arduino-clean

.PHONY: arduino-upload
arduino-upload:
	arduino-cli compile --upload --verify --port="${ARDUINO_PORT}" --fqbn "${ARDUINO_FQDN}" arduino

.PHONY: arduino-clean
arduino-clean:
	rm ./arduino/*.elf
	rm ./arduino/*.hex

.PHONY: client
client:
	npm start

.PHONY: client-install
client-install:
	npm install

.PHONY: client-clean
client-clean:
	rm -r node_modules
	rm dist/*

.PHONY: server
server:
	pipenv run start

.PHONY: server-install
server-install:
	pipenv install

.PHONY: clean
clean: arduino-clean client-clean
