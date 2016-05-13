// **************************************************************************
// File       [ user_cmd.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/04/10 created ]
// **************************************************************************

#include "user_cmd.h"
#include "graph.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <queue>
#include "../lib/tm_usage.h"
using namespace std;
using namespace CommonNs;

TestCmd::TestCmd(const char * const name) : Cmd(name) {
    optMgr_.setShortDes("test");
    optMgr_.setDes("test");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ, "print the string of -s", "[string]");
    opt->addFlag("s");
    optMgr_.regOpt(opt);
}

TestCmd::~TestCmd() {}

bool TestCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (optMgr_.getParsedOpt("s")) {
        printf("%s\n", optMgr_.getParsedValue("s"));
    }
    else
        printf("hello world !!\n");


    return true;
}

ReadGraphCmd::ReadGraphCmd(const char * const name, CmdMgr *cmdMgr) : Cmd(name) {

    cmdMgr_ = cmdMgr;
    optMgr_.setShortDes("read graph");
    optMgr_.setDes("read the graph in verilog format");

    optMgr_.regArg(new Arg(Arg::REQ, "input dot file", "INPUT"));

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

}

ReadGraphCmd::~ReadGraphCmd() {}

bool ReadGraphCmd::exec(int argc, char **argv) {
    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    if (argc < 2) {
        fprintf(stderr, "**ERROR ReadGraphCmd::exec(): ");
        fprintf(stderr, "please specify input file\n");
        return false;
    }

    /*if (argc > 2) {
        fprintf(stderr, "**ERROR ReadGraphCmd::exec(): ");
        fprintf(stderr, "extra argument\n");
        return false;
    }*/

    ifstream gnfile;
    gnfile.open(argv[1]);
    if (!gnfile.is_open()) {
        fprintf(stderr, "**ERROR ReadGraphCmd::exec(): ");
        fprintf(stderr, "no such file\n");
        return false;
    }

    Graph* g = &cmdMgr_->graph_;
    string tmp = g->name;
    g->name = string(strtok(strstr(argv[1], "gn"), "."));

    if (!g->nodes.empty()) {
        vector<Edge *>::iterator itE;
        for (itE = g->edges.begin(); itE != g->edges.end(); itE++) {
            delete *(itE);
            *(itE) = 0;
        }
        map<int, Node *>::iterator itN;
        for (itN = g->nodes.begin(); itN != g->nodes.end(); itN++) {
            delete itN->second;
            itN->second = 0;
        }
        g->edges.clear();
        g->nodes.clear();
        cout << "replace " << tmp << ".dot by " << g->name << ".dot\n";
    }

    char line[256];
    while (!gnfile.eof()) {
        gnfile.getline(line, 256);
        if (strspn(line, "/") == 2)
            continue;
        if (strspn(line, "}") == 1)
            continue;
        if (strspn(line, "graph") == 5)
            //g->name = string(strtok(&line[5], " "));
            continue;
        else {
            int v[2], w = 0;
            v[0] = 0; v[1] = 0;
            for (size_t i = 0; i < strlen(line); i++) {
                if (line[i] == 'v') {
                    int j = ++i;
                    while (line[++j] != ' ');
                    v[w++] = atoi(string(&line[i], j-i).c_str());
                    i = j;
                }
                if (line[i] == '\"') {
                    int j = ++i;
                    while (line[++j] != '\"');
                    w = atoi(string(&line[i], j-i).c_str());
                    break;
                }
            }
            if (w != 0 || v[0] != v[1])
                g->addEdge(v[0], v[1], w);
        }
    }
    cout << "read graph " << g->name << ":\n"
         << "Node = " << g->nodes.size() << "\n"
         << "Edge = " << g->edges.size() << "\n";
    g->sortEdgesOfNode();
    g->sortEdgesByWeight();
    g->init();
    gnfile.close();
    return true;
}

WriteDFSCmd::WriteDFSCmd(const char * const name, CmdMgr *cmdMgr) : Cmd(name) {

    cmdMgr_ = cmdMgr;
    optMgr_.setShortDes("write dfs tree");
    optMgr_.setDes("perform depth first search starting from source node. Then write to a dot file.");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "output file. Default is <input>_dfs.<format>",
                  "OUTPUT");
    opt->addFlag("o");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "source node. Default is 0.",
                  "SOURECENODE");
    opt->addFlag("s");
    optMgr_.regOpt(opt);
}

WriteDFSCmd::~WriteDFSCmd() {}

bool WriteDFSCmd::exec(int argc, char **argv) {

    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    Graph* g = &cmdMgr_->graph_;
    TmUsage tmusg;
    TmStat stat;

    if (g->nodes.empty()) {
        fprintf(stderr, "**ERROR WriteDFSCmd::exec(): ");
        fprintf(stderr, "graph is empty, please read_graph first\n");
        return false;
    }

    int s;
    string name;
    if (optMgr_.getParsedOpt("s")) {
        s = atoi(strtok(optMgr_.getParsedValue("s"), "v"));
        if (g->nodes.find(s) == g->nodes.end()) {
            fprintf(stderr, "**ERROR WriteDFSCmd::exec(): ");
            fprintf(stderr, "invalid source node\n");
            return false;
        }
    }
    else s = 0;

    if (optMgr_.getParsedOpt("o")) {
        name = string(optMgr_.getParsedValue("o"));
    }
    else {
        name = g->name;
        name.append("_dfs.dot");
    }
    cout << "Source node: " << s << "\nOutput name: " << name << "\n"; 

    tmusg.periodStart();

    // initialize
    g->init();
    // build dfs tree
    ofstream o(name.c_str(), ofstream::out);
    o << "graph " << g->name << "_dfs {\n";
    int time = 0;
    time = g->DFS_Visit(g->nodes.find(s)->second, time, o);
    /*map<int, Node*>::iterator itN;
    for (itN = g->nodes.begin(); itN != g->nodes.end(); itN++) {
        if (itN->second->traveled == 0)
            time = g->DFS_Visit(itN->second, time, o);
    }*/
    // transfer the dfs result to output file
    /*for (map<int, Node*>::iterator i = g->nodes.begin();
                                   i != g->nodes.end(); i++) {
        printf("v%d: d = %d, f = %d\n", i->second->id, i->second->d, i->second->f);
    }*/

    tmusg.getPeriodUsage(stat);
    o << "}\n"
      << "// vertices = " << g->nodes.size() << "\n"
      << "// edges = " << time/2 - 1 << "\n"
      << "// source = v" << s << "\n"
      << "// runtime = " << (stat.uTime + stat.sTime) / 1000000.0 << " sec\n"
      << "// memory = " << stat.vmPeak / 1000.0 << " MB\n";
    o.close();

    return true;
}

WriteBFSCmd::WriteBFSCmd(const char * const name, CmdMgr *cmdMgr) : Cmd(name) {

    cmdMgr_ = cmdMgr;
    optMgr_.setShortDes("write bfs tree");
    optMgr_.setDes("perform breadth first search starting from source node.Then write to a dot file.");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "output file. Default is <input>_bfs.<format>",
                  "OUTPUT");
    opt->addFlag("o");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "source node. Default is 0.",
                  "SOURECENODE");
    opt->addFlag("s");
    optMgr_.regOpt(opt);
}

WriteBFSCmd::~WriteBFSCmd() {}

bool WriteBFSCmd::exec(int argc, char **argv) {

    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    Graph* g = &cmdMgr_->graph_;
    TmUsage tmusg;
    TmStat stat;

    if (g->nodes.empty()) {
        fprintf(stderr, "**ERROR WriteBFSCmd::exec(): ");
        fprintf(stderr, "graph is empty, please read_graph first\n");
        return false;
    }

    int s;
    string name;
    if (optMgr_.getParsedOpt("s")) {
        s = atoi(strtok(optMgr_.getParsedValue("s"), "v"));
        if (g->nodes.find(s) == g->nodes.end()) {
            fprintf(stderr, "**ERROR WriteBFSCmd::exec(): ");
            fprintf(stderr, "invalid source node\n");
            return false;
        }
    }
    else s = 0;

    if (optMgr_.getParsedOpt("o")) {
        name = string(optMgr_.getParsedValue("o"));
    }
    else {
        name = g->name;
        name.append("_bfs.dot");
    }
    cout << "Source node: " << s << "\nOutput name: " << name << "\n"; 

    tmusg.periodStart();

    // initialize
    g->init();
    Node* sn = g->nodes.find(s)->second;
    sn->traveled = true;
    sn->d = 0;
    queue<Node*> Q;
    Q.push(sn);
    // build bfs tree
    ofstream o(name.c_str(), ofstream::out);
    o << "graph " << g->name << "_bfs {\n";
    int time = 0;
    while (!Q.empty()) {
        Node* u = Q.front();
        Q.pop();
        for (vector<Edge*>::iterator i = u->edge.begin();
                                     i != u->edge.end(); i++) {
            Node* v = (*i)->getNeighbor(u);
            if (v->traveled == false) {
                time++;
                o << "v" << u->id << " -- v" << v->id
                  << " [label = \"" << (*i)->weight << "\"];\n";
                v->traveled = true;
                v->d = u->d + 1;
                v->prev = u;
                Q.push(v);
            }
        }
    }
    // transfer the dfs result to output file
    /*for (map<int, Node*>::iterator i = g->nodes.begin();
                                   i != g->nodes.end(); i++) {
        printf("v%d: d = %d, f = %d\n", i->second->id, i->second->d, i->second->f);
    }*/

    tmusg.getPeriodUsage(stat);
    o << "}\n"
      << "// vertices = " << g->nodes.size() << "\n"
      << "// edges = " << time << "\n"
      << "// source = v" << s << "\n"
      << "// runtime = " << (stat.uTime + stat.sTime) / 1000000.0 << " sec\n"
      << "// memory = " << stat.vmPeak / 1000.0 << " MB\n";
    o.close();

    return true;
}

WriteDijkstraCmd::WriteDijkstraCmd(const char * const name, CmdMgr *cmdMgr) : Cmd(name) {

    cmdMgr_ = cmdMgr;
    optMgr_.setShortDes("write dijkstra for SSSP");
    optMgr_.setDes("Perform Dijkstra’s algorithm to find the shortest path from sourcenode to endnode.Then write to a dot file.");

    Opt *opt = new Opt(Opt::BOOL, "print usage", "");
    opt->addFlag("h");
    opt->addFlag("help");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "output file. Default is <input>_dijkstra.<format>",
                  "OUTPUT");
    opt->addFlag("o");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "source node. Default is 0.",
                  "SOURECENODE");
    opt->addFlag("f");
    opt->addFlag("from");
    optMgr_.regOpt(opt);

    opt = new Opt(Opt::STR_REQ,
                  "end node. Default is |V|-1.",
                  "ENDNODE");
    opt->addFlag("t");
    opt->addFlag("to");
    optMgr_.regOpt(opt);
}

WriteDijkstraCmd::~WriteDijkstraCmd() {}

bool WriteDijkstraCmd::exec(int argc, char **argv) {

    optMgr_.parse(argc, argv);

    if (optMgr_.getParsedOpt("h")) {
        optMgr_.usage();
        return true;
    }

    Graph* g = &cmdMgr_->graph_;
    TmUsage tmusg;
    TmStat stat;

    if (g->nodes.empty()) {
        fprintf(stderr, "**ERROR WriteDijkstraCmd::exec(): ");
        fprintf(stderr, "graph is empty, please read_graph first\n");
        return false;
    }

    int s, e;
    string name;
    if (optMgr_.getParsedOpt("f")) {
        s = atoi(strtok(optMgr_.getParsedValue("f"), "v"));
        if (g->nodes.find(s) == g->nodes.end()) {
            fprintf(stderr, "**ERROR WriteDijkstraCmd::exec(): ");
            fprintf(stderr, "invalid source node\n");
            return false;
        }
    }
    else s = 0;

    if (optMgr_.getParsedOpt("t")) {
        e = atoi(strtok(optMgr_.getParsedValue("t"), "v"));
        if (g->nodes.find(e) == g->nodes.end()) {
            fprintf(stderr, "**ERROR WriteDijkstraCmd::exec(): ");
            fprintf(stderr, "invalid end node\n");
            return false;
        }
    }
    else e = g->nodes.size() - 1;

    if (optMgr_.getParsedOpt("o")) {
        name = string(optMgr_.getParsedValue("o"));
    }
    else {
        name = g->name;
        name.append("_dijkstra.dot");
    }
    cout << "Source node: " << s << "\nEnd node:    " << e
         << "\nOutput name: " << name << "\n"; 

    tmusg.periodStart();

    // initialize
    g->init();
    Node* sn = g->nodes.find(s)->second;
    sn->traveled = true;
    sn->d = 0;
    Node* en = g->nodes.find(e)->second;
    vector<Node*> S, Q;
    for (map<int, Node*>::iterator i = g->nodes.begin();
                                   i != g->nodes.end(); i++)
        Q.push_back(i->second);
    // build dijkstra
    while (!Q.empty()) {
        sort(Q.begin(), Q.end(), nodeComp);
        Node* u = Q.front();
        Q.erase(Q.begin());
        S.push_back(u);
        if (u == en) break;
        for (vector<Edge*>::iterator i = u->edge.begin();
                                     i != u->edge.end(); i++) {
            Node* v = (*i)->getNeighbor(u);
            if (v->d > u->d + (*i)->weight) {
                v->d = u->d + (*i)->weight;
                v->prev = u;
            }
        }
    }
    // transfer the dfs result to output file
    tmusg.getPeriodUsage(stat);
    ofstream o(name.c_str(), ofstream::out);
    o << "graph " << g->name << "_dijkstra {\n";
    vector<string> sss;
    int weight = 0;
    while (en != sn) {
        for (vector<Edge*>::iterator i = en->edge.begin();
                                     i != en->edge.end(); i++)
            if ((*i)->getNeighbor(en) == en->prev) {
                stringstream ss;
                ss << "v" << en->prev->id << " -- v" <<  en->id
                   << " [label = \"" << (*i)->weight << "\"];\n";
                sss.push_back(ss.str());
                weight += (*i)->weight;
                en = en->prev;
                break;
            }
    }
    int time = 0;
    for (vector<string>::iterator i = sss.end() - 1; time < sss.size(); i--) {
        o << (*i);
        time++;
    }
    o << "}\n"
      << "// vertices = " << time + 1 << "\n"
      << "// edges = " << time << "\n"
      << "// source = v" << s << "\n"
      << "// end = v" << e << "\n"
      << "// total_weight = " << weight << "\n"
      << "// runtime = " << (stat.uTime + stat.sTime) / 1000000.0 << " sec\n"
      << "// memory = " << stat.vmPeak / 1000.0 << " MB\n";
    o.close();

    return true;
}
