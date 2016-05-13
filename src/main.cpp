// **************************************************************************
// File       [ main.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ this is a simple tutorial to demo the command manager ]
// Date       [ 2012/04/10 created ]
// **************************************************************************

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

#include "read_verilog.h"

int main( int argc, char **argv ) {
    if( argc != 2 ) {
        fprintf( stderr, "**ERROR main(): " );
        fprintf( stderr, "wrong argument\n" );
        return 0;
    }
    read_verilog( argv[1] );
    return 0;
}

/*#include "sys_cmd.h"
#include "user_cmd.h"
#include "graph.h"
using namespace CommonNs;

void initOpts(OptMgr *mgr);
void initCmds(CmdMgr *mgr);

int main(int argc, char **argv) {

    OptMgr *optMgr = new OptMgr;
    CmdMgr *cmdMgr = new CmdMgr;

    initOpts(optMgr);
    initCmds(cmdMgr);

    // if -h option is used, show help
    optMgr->parse(argc, argv);
    if (optMgr->getParsedOpt("h")) {
        optMgr->usage();
        exit(0);
    }


    //  welcome message
    cout << "        Welcome to GraphLab \n" ;  
    cout << "       (c) NTUEE Algorithms \n" ;  
    cout << "        V 2012.4  \n" ;  


    // if -f is used, source the file in batch mode
    CmdMgr::Result res = CmdMgr::SUCCESS;
    if (optMgr->getParsedOpt("f")) {
        char cmd[256];
        strcpy(cmd, "source ");
        strcat(cmd, optMgr->getParsedValue("f"));
        res = cmdMgr->exec(cmd);
    }


    // keep looping the interactive shell
    while (res != CmdMgr::EXIT) {
        res = cmdMgr->read();
        if (res == CmdMgr::NOT_EXIST) {
            fprintf(stderr, "**ERROR main(): command ");
            fprintf(stderr, "`%s' not found\n", cmdMgr->getErrorStr());
            continue;
        }
    }

    return 0;
}

void initOpts(OptMgr *mgr) {
    // set program information
    mgr->setName("test");
    mgr->setShortDes("test based");
    mgr->setDes("test based");

    // register options
    Opt *opt = new Opt(Opt::BOOL, "print this usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    mgr->regOpt(opt);

    opt = new Opt(Opt::STR_REQ, "execute command file at startup", "FILE");
    opt->addFlag("f");
    mgr->regOpt(opt);
}

void initCmds(CmdMgr *mgr) {
  // user interface
  mgr->setComment("#");            // comment
  mgr->setPrompt("graphlab> ");  // set prompt
  mgr->setColor(CmdMgr::YELLOW); // set prompt color

  // declear system commands here
  Cmd *listCmd   = new SysListCmd("ls");
  Cmd *cdCmd     = new SysCdCmd("cd");
  Cmd *exitCmd   = new SysExitCmd("exit", mgr);
  Cmd *quitCmd   = new SysExitCmd("quit", mgr);
  Cmd *sourceCmd = new SysSourceCmd("source", mgr);
  Cmd *helpCmd   = new SysHelpCmd("help", mgr);
  Cmd *sysDotCmd = new SysDotCmd("dot");
  Cmd *sysDisplayCmd   = new SysDisplayCmd("display");


  // register  system commands here
  mgr->regCmd(listCmd);
  mgr->regCmd(cdCmd);
  mgr->regCmd(exitCmd);
  mgr->regCmd(quitCmd);
  mgr->regCmd(sourceCmd);
  mgr->regCmd(helpCmd);
  mgr->regCmd(sysDotCmd);
  mgr->regCmd(sysDisplayCmd);

  // add your command here
  Cmd *testCmd           = new TestCmd("test");
  Cmd *read_graphCmd     = new ReadGraphCmd("read_graph", mgr);
  Cmd *write_tree_dfsCmd = new WriteDFSCmd("write_tree_dfs", mgr);
  Cmd *write_tree_bfsCmd = new WriteBFSCmd("write_tree_bfs", mgr);
  Cmd *write_dijkstraCmd = new WriteDijkstraCmd("write_dijkstra", mgr);
  mgr->regCmd(testCmd);
  mgr->regCmd(read_graphCmd);
  mgr->regCmd(write_tree_dfsCmd);
  mgr->regCmd(write_tree_bfsCmd);
  mgr->regCmd(write_dijkstraCmd);

}*/

