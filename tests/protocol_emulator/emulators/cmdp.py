import emulators.emulator


class CMDP_Emulator(emulators.emulator.Emulator):
    class MessageConstants:
        class Header:
            Start = R"<CMDP_H>"
            End = R">CMDP_H<"

        class Data:
            Start = R"<CMDP_D>"
            End = R">CMDP_D<"

        Tail = R"<CMDP_T>"

    def __init__(self, *args):
        """
        See class emulators.emulator.Emulator class for description.
        """
        super(CMDP_Emulator, self).__init__(*args)

    def transmitSession(self, diagram_title, data_line_titles, content_of_the_data_messages):
        """
        See class emulators.emulator.Emulator class for description.
        """
        self._transmitHeader(diagram_title, data_line_titles)
        for data_message_content in content_of_the_data_messages:
            self._transmitData(data_message_content)
        self._transmitTail()

    def _transmitHeader(self, diagram_title, data_line_titles):
        self._transmit_line(CMDP_Emulator.MessageConstants.Header.Start)
        if diagram_title:
            self._transmit_line("<" + diagram_title + ">")
        line_to_send = ""
        for key, value in data_line_titles.items():
            line_to_send += "{}:{},".format(key, value)
        self._transmit_line(line_to_send)
        self._transmit_line(CMDP_Emulator.MessageConstants.Header.End)

    def _transmitData(self, data_message_content):
        self._transmit_line(CMDP_Emulator.MessageConstants.Data.Start)
        for measurement_point_data in data_message_content:
            line_to_send = ""
            for key, value in measurement_point_data.items():
                line_to_send += "{}:{},".format(key, value)
            self._transmit_line(line_to_send)
        self._transmit_line(CMDP_Emulator.MessageConstants.Data.End)

    def _transmitTail(self):
        self._transmit_line(CMDP_Emulator.MessageConstants.Tail)
