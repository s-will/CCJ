#ifndef PSEUDO_LOOP_H_
#define PSEUDO_LOOP_H_
#include <stdio.h>
#include <string.h>
#include "h_struct.h"
#include "h_common.h"
#include "V_final.h"
#include "VM_final.h"
#include "trace_arrow.h"
#include "candidate_list.h"

class VM_final;
class V_final;
class pseudo_loop{

public:
    // constructor
    pseudo_loop(char *seq, V_final *V, s_hairpin_loop *H, s_stacked_pair *S, s_internal_loop *VBI, VM_final *VM);

    // destructor
    ~pseudo_loop();

    bool pl_debug = false;      // Print general debug information. Prints a lot
    bool cl_debug = false;      // Print candidate list debug information
    bool node_debug = false;    // Print back-tracking debug information
    bool f_pair_debug = false;  // Print information for every time it pairs

    bool sparsify;
    bool avoid_candidates = true;  // Don't create a trace arrow if will point to an already existing candidate
    bool use_garbage_collection;   // Garbage collect unneccesary trace arrows
    bool use_compactify = false;   // Reallocate trace arrow arrays if they are holding too much memory they don't need.
                                   // compactify is automatically turned on if garbage collection is on, because garbage collection is useless without it.

    // These are set to nb_nucleotides if non-sparse
    int MOD_2 = 2;
    int MOD_MAXLOOP = MAXLOOP;

    // 0 will not print, 1 will print basic information, 2 will print verbose information
    char print_ta_info;    // Trace Arrow numbers - enabled with -pta or -pta-v
    char print_cl_info;    // Candidate List numbers - enabled with -pcl or pcl-v

    // calls compute_energies_sparse or nonsparse
    void compute_energies(int i, int j);

    // SP means sparse
    // NS means non-sparse
    void compute_energies_sp(int i, int j);
    void compute_energies_ns(int i, int j);

    int get_energy(int i, int j);
    // in order to be able to check the border values consistantly
    // I am adding these get functions

    // getter functions for nested substructures

    // nested substr in a regular multiloop
    //int get_WM(int i, int j); // in base pair maximization, there is no difference between the two
    //int get_WMP(int i, int j);

    // nested substr in a multiloop that spans a band
    int get_WB(int i, int j); // in base pair maximization, there is no difference between the two
    int get_WBP(int i, int j);

    // nested substr in a pseudoloop
    int get_WP(int i, int j); // in base pair maximization, there is no difference between the two
    int get_WPP(int i, int j);

    int get_P(int i, int j);
    int get_PK(int i,int j, int k, int l);
    int get_PL(int i,int j, int k, int l);
    int get_PR(int i,int j, int k, int l);
    int get_PM(int i,int j, int k, int l);
    int get_PO(int i,int j, int k, int l);
    
    int get_PfromL(int i, int j, int k, int l);
    int get_PfromR(int i, int j, int k, int l);
    int get_PfromM(int i, int j, int k, int l);
    int get_PfromO(int i, int j, int k, int l);


    int get_PLiloop(int i,int j, int k, int l);
    //int get_PLiloop5(int i,int j, int k, int l,int s);
    int get_PLmloop(int i,int j, int k, int l);
    int get_PLmloop10(int i,int j, int k, int l);
    int get_PLmloop01(int i,int j, int k, int l);
    int get_PLmloop00(int i,int j, int k, int l);

    int get_PRiloop(int i,int j, int k, int l);
    //int get_PRiloop5(int i,int j, int k, int l,int s);
    int get_PRmloop(int i,int j, int k, int l);
    int get_PRmloop10(int i,int j, int k, int l);
    int get_PRmloop01(int i,int j, int k, int l);
    int get_PRmloop00(int i,int j, int k, int l);

    int get_PMiloop(int i,int j, int k, int l);
    //int get_PMiloop5(int i,int j, int k, int l,int s);
    int get_PMmloop(int i,int j, int k, int l);
    int get_PMmloop10(int i,int j, int k, int l);
    int get_PMmloop01(int i,int j, int k, int l);
    int get_PMmloop00(int i,int j, int k, int l);

    int get_POiloop(int i,int j, int k, int l);
    //int get_POiloop5(int i,int j, int k, int l,int s);
    int get_POmloop(int i,int j, int k, int l);
    int get_POmloop10(int i,int j, int k, int l);
    int get_POmloop01(int i,int j, int k, int l);
    int get_POmloop00(int i,int j, int k, int l);



    // int is_weakly_closed(int i, int j);
    //int is_empty_region(int i, int j);

    // Hosna, Feb 18, 2014
    // I am changing the backtrack function such that it does not deal with structure
    // instead it only fills the minimum_fold array, f, and passes it to W_final
    // then in W_final one pass over f, will create the structure in dot bracket format
    // This is the solution I found for the problem of not knowing what kind of brackets and
    // how many different brackets to use when fillinf f and structure at the same time in pseudoloop.cpp

    //void back_track(char *structure, minimum_fold *f, seq_interval *cur_interval);
    void back_track(minimum_fold *f, seq_interval *cur_interval);
    void back_track_ns(minimum_fold *f, seq_interval *cur_interval);
    void back_track_sp(minimum_fold *f, seq_interval *cur_interval);
    // Used in sparse back tracking
    void bt_WB(int i,int l);
    void bt_WBP(int i, int l);
    void bt_WP(int i, int l);
    void bt_WPP(int i, int l);

    void set_stack_interval(seq_interval *stack_interval);
    seq_interval *get_stack_interval(){return stack_interval;}
    //char *get_structure(){return structure;}
    minimum_fold *get_minimum_fold(){return f;}


private:

    int nb_nucleotides;
    int *int_sequence;
    char *sequence;
    //char *restricted;

    s_hairpin_loop *H;      // hairpin loop object
    s_stacked_pair *S;      // stack pair object
    s_internal_loop *VBI;   // internal loop object
    VM_final *VM;	        // multi loop object
    V_final *V;		        // the V object

    //h_str_features *fres;
    seq_interval *stack_interval;
    char *structure;
    minimum_fold *f;

    //int needs_computation; // This global variable is used so that we don't compute energies in backtracking


    //int *WP;				// the loop inside a pseudoknot (in general it looks like a W but is inside a pseudoknot) // in base pair maximization, there is no difference between the two
    int *WPP;				// similar to WP but has at least one base pair
    // Hosna, Feb 14, 2014
    // WM and Vm recurrences in HFold and CCJ are similar, so I am keeping them here for CCJ similar to HFold
    // i.e. WM is implemented in VM_final
    //int *WM;				// the loop inside a regular multiloop // in base pair maximization, there is no difference between the two
    //int *WMP;				// similar to WM but has at least one base pair
    //int *WB;				// the loop inside a multiloop that spans a band // in base pair maximization, there is no difference between the two
    int *WBP;				// similar to WB but has at least one base pair

    int *P;					// the main loop for pseudoloops and bands
    int **PK;				// MFE of a TGB structure over gapped region [i,j] U [k,l]
    int ***PL;				// MFE of a TGB structure s.t. i.j is paired
    int **PR;				// MFE of a TGB structure s.t. k.l is paired
    int **PM;				// MFE of a TGB structure s.t. j.k is paired
    int ***PO;				// MFE of a TGB structure s.t. i.l is paired

    // transition recurrences
    int ***PfromL;
    int **PfromR;
    int **PfromM;
    int ***PfromO;

    // internal loops and multi loops that span a band
    //int **PLiloop;
    //int ***PLiloop5;
    //int **PLmloop;
    int ***PLmloop10;
    int ***PLmloop01;
    int **PLmloop00;


    //int **PRiloop;
    //int ***PRiloop5;
    //int **PRmloop;
    int **PRmloop10;
    int **PRmloop01;
    int **PRmloop00;


    //int **PMiloop;
    //int ***PMiloop5;
    //int **PMmloop;
    int **PMmloop10;
    int **PMmloop01;
    int **PMmloop00;


    //int **POiloop;
    //int ***POiloop5;
    //int **POmloop;
    int ***POmloop01;
    int ***POmloop10;
    int **POmloop00;

    // Ian Wark Jan 23, 2017
    // Candidate Lists (candidate type is in h_struct.h)
    // 3D arrays pointing to linked lists where candidates point to the next in the list
    // Actually implemented as 2D arrays accessed by [j][kl]
    candidate_list *PLmloop00_CL;
    candidate_list *POmloop00_CL;
    candidate_list *PfromL_CL;
    candidate_list *PfromO_CL;
    candidate_PK **PK_CL;

    void push_candidate_PK(int d, int j, int k, int l, int w)
    {
        const candidate_PK *next = PK_CL[l];
        candidate_PK *toAdd = new candidate_PK(d,j,k,w,next);

        PK_CL[l]=toAdd;
    }

    // returns whether there is a candidate in that list at that location
    bool is_candidate(int i, int j, int k, int l, candidate_PK **CL) const
    {
        return (find_candidate(i,j,k,l,CL) != nullptr);
    }

    /**
     * Find candidate in PK candidate list CL
     *
     * @param i
     * @param j
     * @param k
     * @param l
     * @returns if failure returns nullptr, else candidate_PK
     */
    const candidate_PK* find_candidate(int i, int j, int k, int l, candidate_PK **CL) const {
        int prev_j = 0, prev_d = 0, prev_k = 0;

        // j is always ascending (or equal)
        // d is always descending (or equal)
        // k is always ascending (or equal)
        const candidate_PK *c = CL[l];
        /// TODO does this checking stuff actually save time or do the checks take too long
        /// probably more of a difference on a long one?
        while (c != NULL && (c->j() <= i || c->d() >= j || c->k() <= k)) {
            //assert(c->j() > prev_j || c->d() < prev_d || c->k() > prev_k);
            //prev_j = c->j(); prev_d = c->d(); prev_k = c->k();

            if (c->j() == i && c->d() == j && c->k() == k) { return c; }

            c = c->get_next();
        }
        // No candidate found in CL with that i
        return nullptr;
    }

    /**
    * Looks through candidate list CL for energy value e
    *
    * @param i
    * @param j
    * @param k
    * @param l
    * @param srctype - type of source matrix (P_PL, P_PfromL, etc.)
    * @param tgttype - type of target matrix (P_PL, P_PfromL, etc.)
    * @param e - energy value
    * @param CL - candidate list to look through
    */
    void trace_candidate(int i, int j, int k, int l, char srctype, char tgttype, energy_t e, candidate_list *CL);
    void trace_candidate_continue(int i, int j, int k, int l, int m, int n, int o, int p, char srctype, char tgttype, const candidate *c);


    /**
    * Looks through candidate list CL for energy value e
    *
    * @param i
    * @param j
    * @param k
    * @param l
    * @param srctype - type of source matrix (P_P, P_PK, etc.)
    * @param tgttype - type of target matrix (P_P, P_PK, etc.)
    * @param e - energy value
    * @param CL - candidate list to look through
    */
    void trace_candidate(int i, int j, int k, int l, char srctype, char tgttype, energy_t e, candidate_PK **CL);


    // Ian Wark Feb 8, 2017
    // Trace arrows
    MasterTraceArrows *ta;

public:
    void print_PK_CL_size();
    // prints information on candidate lists
    void print_CL_sizes();
    // prints infromation on candidate lists for each list
    void print_CL_sizes_verbose();

    void gc_trace_arrows(size_t i) {
        //printf("gc_trace_arrows(%d)\n",i);
        if ( sparsify && use_garbage_collection && i+MAXLOOP+1 <= nb_nucleotides) {
            ta->garbage_collect(i + MAXLOOP + 1);
            // compactify is needed to get any bonus from garbage collection
            ta->compactify();
        }
    }

    // how useful is compactify for anything other than trace arrows?
    // moderately. Space gain is not huge but neither is slow down
    void compactify() {
        if (sparsify && use_compactify) {
            PfromL_CL->compactify();
            PfromO_CL->compactify();
            PLmloop00_CL->compactify();
            POmloop00_CL->compactify();

            if (!use_garbage_collection)
                ta->compactify();
        }
    }

    // prints basic information on trace arrows
    void print_ta_sizes() {
        ta->print_ta_sizes();
    }

    // prints information on trace arrows for each trace arrow container
    void print_ta_sizes_verbose() {
        ta->print_ta_sizes_verbose();
    }

private:
    void get_PK_CL_size(int &candidates, int &empty_lists);
    //int *weakly_closed;		// the array which is keeping track of which regions are weakly closed
    //int *not_paired_all;	// the array which keeps track of empty regions
    int *index;				// the array to keep the index of two dimensional arrays like weakly_closed

    // functions to allocate space for the arrays
    // for sparse version
    void allocate_space_sparse();

    void allocate_space_nonsparse();

    //void compute_WM(int i, int j); // in base pair maximization, there is no difference between the two
    //void compute_WMP(int i, int l);
    //void compute_WB(int i, int j); // in base pair maximization, there is no difference between the two
    void compute_WBP(int i, int l);
    //void compute_WP(int i, int j); // in base pair maximization, there is no difference between the two
    void compute_WPP(int i, int l);

    void compute_P_sp(int i, int l);
    void compute_P_ns(int i, int l);
    void compute_PK(int i,int j, int k, int l);
    void compute_PL(int i,int j, int k, int l);
    void compute_PR(int i,int j, int k, int l);
    void compute_PM(int i,int j, int k, int l);
    void compute_PO(int i,int j, int k, int l);


    void compute_PfromL_sp(int i, int j, int k, int l);
    void compute_PfromL_ns(int i, int j, int k, int l);
    void compute_PfromR(int i, int j, int k, int l);
    void compute_PfromM(int i, int j, int k, int l);
    void compute_PfromO_sp(int i, int j, int k, int l);
    void compute_PfromO_ns(int i, int j, int k, int l);


    //void compute_PLiloop(int i,int j, int k, int l);
    //void compute_PLiloop5(int i,int j, int k, int l,int s);
    //void compute_PLmloop(int i,int j, int k, int l);
    void compute_PLmloop10_sp(int i,int j, int k, int l);
    void compute_PLmloop10_ns(int i,int j, int k, int l);
    void compute_PLmloop01(int i,int j, int k, int l);
    void compute_PLmloop00_sp(int i,int j, int k, int l);
    void compute_PLmloop00_ns(int i,int j, int k, int l);

    //void compute_PRiloop(int i,int j, int k, int l);
    //void compute_PRiloop5(int i,int j, int k, int l,int s);
    //void compute_PRmloop(int i,int j, int k, int l);
    void compute_PRmloop10(int i,int j, int k, int l);
    void compute_PRmloop01(int i,int j, int k, int l);
    void compute_PRmloop00(int i,int j, int k, int l);

    //void compute_PMiloop(int i,int j, int k, int l);
    //void compute_PMiloop5(int i,int j, int k, int l,int s);
    //void compute_PMmloop(int i,int j, int k, int l);
    void compute_PMmloop10(int i,int j, int k, int l);
    void compute_PMmloop01(int i,int j, int k, int l);
    void compute_PMmloop00(int i,int j, int k, int l);

    //void compute_POiloop(int i,int j, int k, int l);
    //void compute_POiloop5(int i,int j, int k, int l,int s);
    //void compute_POmloop(int i,int j, int k, int l);
    void compute_POmloop10_sp(int i,int j, int k, int l);
    void compute_POmloop10_ns(int i,int j, int k, int l);
    void compute_POmloop01(int i,int j, int k, int l);
    void compute_POmloop00_sp(int i,int j, int k, int l);
    void compute_POmloop00_ns(int i,int j, int k, int l);

    // I have to calculate the e_stP in a separate function
    int get_e_stP(int i, int j);
    int get_e_intP(int i,int ip, int jp, int j);

    /**
    * @param location of source
    * @param source type
    */
    void trace_continue(int i, int j, int k, int l, char srctype, energy_t e);

    void trace_update_f(int i, int j, int k, int l, char srctype);
    void trace_update_f_with_target(int i, int j, int k, int l, char srctype, char tgttype);

    // Takes a char denoting type and prints out a string for that type
    // P_POmloop10 prints "POmloop10" instead of '-', its char representation
    void print_type(char type);

    // used for backtracking
    void insert_node (int i, int j, char type);//, seq_interval *stack_interval);
    // Hosna, Feb 15, 2014
    // added the following function for CCJ use
    // overloaded functions of insert_node
    void insert_node(int i, int j, int k, int l, char type);
    void insert_node(int i, int j, int k, int l, int s, char type);
};
#endif /*PSEUDO_LOOP_H_*/
