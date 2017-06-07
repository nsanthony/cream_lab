								2004/2/27 SYZ

class CreamFile
===============
1. open/close/rewind a file.
2. give current file position.
3. file size.
4. read an event and fill device buffers.
   the data in the buffers stay until next reading.
   data buffers are passed to DEvent object for further processing.
5. backward compatibility is maintained.


class CreamFileHeader
=====================
1. read/write header information.
2. check header information for readability.
3. print header information to a file pointer.


class CreamFileTail
===================
1. read/write tail information.
2. print header information to a file pointer.

class DEvent
============
1. unpack data from various devices.
2. provide convenient access to event data.
