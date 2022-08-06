# Resin-Doser
Autofill a resin vat during big prints on an SLA printer

This is an arduino program made to drive an enable direction and step driver for a peristaltic pump.
The pump should push resin into an SLA printer vat at regular doses over the couse of the print.
Currently print settings are configured in the firmware, but buttons to start cycle, stop, and manualy drive the pump are implemented.
Lights show on, off, and running. Check pinout section of the code for example pinout. Buttons should connect to pins and to ground for the arduino input pullup function.
