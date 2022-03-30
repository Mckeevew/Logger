# Logger

Simple logging library I made for myself. Will print to the consol and a results.log file. This logger attempts to replicate the style and syntax of std::out.

# Usage

//Wipe Logger File

Logger::Init();

Log(Info) << "Info Print";

Log(Warn) << "Warn Print";

Log(Error) << "Error Print";

bool isTrue = false;
Assert( isTrue ) << "isTrue was not true. Trigger program crash for debugger.";
