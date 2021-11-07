# Mock FTP client

FTP-like client for transfering files. The client provides 4 commands: ls, get, put, and exit.

## Subheader

The client performs ls, get, put or exit by constructing a Command, and sending it to the server, and then
sending or receiving a Payload struct, followed by text or the contents of a file. The CC LS, CC PUT, CC GET,
and CC EXIT are used in the code field of the Command struct, to indicate what command is being performed.

The Payload indicates whether it will be followed by text, or the contents of the file. The code field of the
Payload is either PL TXT or PL FILE, and the length field indicates how many bytes are being sent.