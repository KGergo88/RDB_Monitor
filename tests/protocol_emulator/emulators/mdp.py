import emulators.emulator


class MDP_Emulator(emulators.emulator.Emulator):
    class MessageConstants:
        class Session:
            Start = R"<<<START>>>"
            End = R"<<<END>>>"

    def __init__(self, *args):
        """
        See class emulators.emulator.Emulator class for description.
        """
        super(MDP_Emulator, self).__init__(*args)

    def transmitSession(self, diagram_title, data_line_titles, content_of_the_data_messages):
        """
        See class emulators.emulator.Emulator class for description.
        """
        self._transmitHeader(diagram_title, data_line_titles)
        for data_message_content in content_of_the_data_messages:
            self._transmitData(data_message_content)
        self._transmitTail()

    def _transmitHeader(self, diagram_title, data_line_titles):
        self._transmit_line(MDP_Emulator.MessageConstants.Session.Start)
        if diagram_title:
            self._transmit_line("<" + diagram_title + ">")
        line_to_send = ""
        for data_line_title in data_line_titles.values():
            line_to_send += "{},".format(data_line_title)
        self._transmit_line(line_to_send)

    def _transmitData(self, data_message_content):
        for measurement_point_data in data_message_content:
            line_to_send = ""
            for value in measurement_point_data.values():
                line_to_send += "{},".format(value)
            self._transmit_line(line_to_send)

    def _transmitTail(self):
        self._transmit_line(MDP_Emulator.MessageConstants.Session.End)
