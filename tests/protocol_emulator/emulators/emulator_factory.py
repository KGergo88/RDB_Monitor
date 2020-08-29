import logging

import emulators.mdp
import emulators.cmdp


# The protocols that are supported
AVAILABLE_PROTOCOLS = {
    "MDP" : emulators.mdp.MDP_Emulator,
    "CMDP" : emulators.cmdp.CMDP_Emulator,
}


def createEmulator(protocol, *args):
    emulator = None

    if protocol in AVAILABLE_PROTOCOLS:
        emulator_constructor = AVAILABLE_PROTOCOLS[protocol]
        if emulator_constructor:
            emulator = emulator_constructor(*args)
        else:
            logging.fatal("The emulator was not yet implemented for this protocol: {}".format(protocol))
            raise NotImplementedError()
    else:
        logging.fatal("Unexpected protocol value: {}".format(protocol))
        raise ValueError()

    return emulator
