// **************************************************************************
// File       [ user_cmd.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2016/04/26 created ]
// **************************************************************************

#ifndef _USER_CMD_H_
#define _USER_CMD_H_

#include "../lib/cmd.h"
#include "graph.h"

namespace CommonNs {

class TestCmd : public Cmd {
public:
         TestCmd(const char * const name);
         ~TestCmd();

    bool exec(int argc, char **argv);
};

class ReadGraphCmd : public Cmd {
public:
         ReadGraphCmd(const char * const name, CmdMgr *mgr);
         ~ReadGraphCmd();

    bool exec(int argc, char **argv);

private:
    CmdMgr *cmdMgr_;
};

class WriteDFSCmd : public Cmd {
public:
         WriteDFSCmd(const char * const name, CmdMgr *mgr);
         ~WriteDFSCmd();

    bool exec(int argc, char **argv);

private:
    CmdMgr *cmdMgr_;
};

class WriteBFSCmd : public Cmd {
public:
         WriteBFSCmd(const char * const name, CmdMgr *mgr);
         ~WriteBFSCmd();

    bool exec(int argc, char **argv);

private:
    CmdMgr *cmdMgr_;
};

class WriteDijkstraCmd : public Cmd {
public:
         WriteDijkstraCmd(const char * const name, CmdMgr *mgr);
         ~WriteDijkstraCmd();

    bool exec(int argc, char **argv);

private:
    CmdMgr *cmdMgr_;
};

};
#endif

