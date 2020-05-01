import logging
import sys
import os
import serial


# socat command to create the serial port pair:
# socat -d -d pty,raw,echo=0 pty,raw,echo=0

# Set this variable to a device (like /dev/pts/3) or to None if the used should enter the name during runtime
DEFAULT_DEVICE = "/dev/pts/3"
# The protocols that are supported by the emulator
AVAILABLE_PROTOCOLS = ("MDP", "CMDP")
# Set this variable to one of the AVAILABLE_PROTOCOLS or to None if the used should enter the name during runtime
DEFAULT_PROTOCOL = AVAILABLE_PROTOCOLS[1]


def get_device(default_device):
    device = default_device
    while device is None:
        device = input("Please enter the name of the device on which the data needs to be sent!\ndevice: ")
        if not os.path.exists(device):
            logging.fatal("The entered device \"{}\" does not exist!".format(device))
            device = None
    return device

def get_protocol(default_protocol):
    protocol = default_protocol
    while protocol is None:
        print("Please enter the number of the protocol with which the data needs to be sent!")
        print("Available protocols:")
        for element in AVAILABLE_PROTOCOLS:
            print("{}-->{}".format(AVAILABLE_PROTOCOLS.index(element), element))
        protocol_index = input("Selected protocol: ")
        try:
            protocol = AVAILABLE_PROTOCOLS[int(protocol_index)]
        except:
            print("Invalid protocol...")
            protocol = None
    return protocol


def main():
    device = get_device(DEFAULT_DEVICE)
    logging.info("Selected device: {}".format(device))

    protocol = get_protocol(DEFAULT_PROTOCOL)
    logging.info("Selected protocol: {}".format(protocol))

    serial_port = None
    try:
        logging.info("Opening serial port")
        serial_port = serial.Serial(device)
        serial_port.write(b'hello')
    except:
        logging.fatal("Could not open serial port")
    finally:
        if serial_port is not None:
            logging.info("Closing serial port")
            serial_port.close()


if "__main__" == __name__:
    # Setting the logging level
    logging.basicConfig(level=logging.DEBUG)
    
    main()
