﻿/*
 * =====================================================================================
 *
 *       Filename:  master.h
 *
 *    Description:  A master class that handles the routine flow of the game
 *                  replacing AIShell routine and make it slightly more efficient
 *                  In particular it can be inherited with newer heuristics and pruning 
 *                  strategy etc.
 *
 *        Created:  2/20/2013 6:34:21 PM
 *       Compiler:  g++
 *
 *         Author:  YU LIU, 
 *
 * =====================================================================================
 */
#ifndef MASTER_H
#define MASTER_H
#include"headers.h"

/*
 * =====================================================================================
 *        Class:  KTree
 *  Description:  Connect K tree that is not balanced in general. 
 *                Note: this class requires fast implementation
 *     
 * =====================================================================================
 */
class KTreeNode
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        KTreeNode ():
            TotalValue(0),pruned(0),depth(999)
        {coord.first=0; coord.second=0;};
        /* constructor */
        // here is out overloaded operator for priority queue
		std::list<std::shared_ptr<KTreeNode>> children;
        //vector provides constant time access and fixed size;
        //however, list provides constant time resizing, reordering and pruning cost
        //in this case the traverse counts per operation is limited by depth which is small
        int TotalValue;
        //the total value of features given by some heuristic traversal operation
        bool pruned;
        //whether or not this node is pruned. If so, terminate search.
        unsigned depth;
        //depth of this node. If this turns out unnecessary then we can get rid of it.
        _mv coord;
        //coordinate of the current node

}; /* -----  end of class KTree  ----- */

typedef std::shared_ptr<KTreeNode> KTreeNode_;
class cmpr_1
{
    public:
        cmpr_1(){};
        bool operator ()(const KTreeNode_ left, const KTreeNode_ right)
        {
            return left->TotalValue < right->TotalValue;
        };
};
/*
 * =====================================================================================
 *        Class:  Master
 *  Description:  the master class and also a base class to be inherited
 * =====================================================================================
 */
class Master
{
    public:
        /* ====================  LIFECYCLE     ======================================= */
        Master ():
            time_limit(4500), //default time limit is 5000 ms
            gravity(0),
			GameTree(new KTreeNode)
        {
            // our tree, don't lose it!
        };                             /* constructor */

        /* ====================  ACCESSORS     ======================================= */

        mv getOneMove(states States,movetype mt=MY_PIECE)
        {
            for (int i=0; i<ROWS; i++)
                for (int j=0; j<COLS; j++)
                {
                    if (States[i][j]==NO_PIECE) 
                        return mv(_mv(i,j),mt);
                }
            return mv(_mv(-1,-1),NO_PIECE);
        };
        std::vector<mv> getAllMoves(states States,movetype mt=MY_PIECE)
        {
            std::vector<mv> out;
            for (int i=0; i<ROWS; i++)
                for (int j=0; j<COLS; j++)
                {
                    if (States[i][j]==NO_PIECE) 
                        out.push_back( mv(_mv(i,j),mt) );
                }
            return out;
        };
        /* ====================  MUTATORS      ======================================= */
        bool listen(bool);
        bool tell_move(mv);

        void tick() {t0=hclock::now();};
        ms tock()
        {
            t1=hclock::now();
            auto timer_t = t1-t0;
			return std::chrono::duration_cast<ms> (timer_t);
        };
        bool time_up()
        {
            auto timer_t=tock();
            if (timer_t.count()>time_limit)
            {
                tick();
                return true;
            }
            else return false;
        };
       
        void update_frontier();
        void IDSearch();
        void expand_one_child(KTreeNode_ parent);
        void expand_all_childern(KTreeNode_ parent);
        
        /* ====================  OPERATORS     ======================================= */
        /* ====================  VIRTUALS     ======================================= */
		virtual void abpruning(){}; // alpha-beta pruning
		virtual void addheuristic(){}; //some heuristic
        
        virtual void main_routine()
        {
            while (!listen(1));
            auto m=getOneMove(GameStates);
            tell_move(m);
            while (listen(false))
            {
                m=getOneMove(GameStates);
                tell_move(m);
            }
        };

    protected:
        /* ====================  DATA MEMBERS  ======================================= */
        hclock::time_point t0, t1;
        int time_limit;
        int COLS,ROWS;
        int K;
        mv lastmove;
        bool gravity;
        std::priority_queue<KTreeNode_,std::vector<KTreeNode_>,cmpr_1> Frontier;
        // our priority tree frontier which is maintained via vector for constant time access
        // note that the frontier is quite small in most cases (it's the immediate candidates
        // for next move)
        KTreeNode_ GameTree;
        // The main tree
        states GameStates, NewStates;
        // std vector does deep copy by default so a reassignment can be used to make copy
        // this is a state, NOT the search tree and not describe the search space. constant size
        

        

}; /* -----  end of class Master  ----- */
#endif
