#include "stdafx.h"

// c++ includes
#include <stdio.h>
#include <string.h>

// corsim related includes
#include "corwin.h"
#include "corsim.h"
#include "netsim.h"

// RTE related includes
#include "network.h"
#include "sensors.h"
#include "commons.h"

// initialize global variables declared in sensors.h
char out_buf[132] = { '\0' };
int log_level = 1; // 0 -> zero; 1 -> medium; 2 -> maximum
bool is_log_active = true;
bool write_others = true; // whether to write or not additional output files
int end_of_init = 0;
int prev_init = 0;
int prev_time = 0;

// initialize a pointer to the network object used by the functions in this file
CNetwork *network = NULL;

DLL_EXPORT void __stdcall on_initialize()
{
    prev_init = 0;
    end_of_init = 0;

    sprintf(out_buf, "Start %s function.", "ON_INITIALIZE()");
    OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);

    // copy the CORSIM input file name from the imported string to a CString (used the imported string length just in case the string is notNULLterminated)
    CString input_file_str(file_path, file_path_len);

    // create network
    network = new CNetwork(input_file_str);

    // read the traf file
    network->readInputFile();

    // print elements found
    if (is_log_active && log_level == 2)
    {
        network->printNodes();
        // network->printLinks(); // FIXME: lancia un'eccezione
        // network->printLanes();
        // network->printDetectors();
    }

    // setup the output file for detectors
    network->setupOutputProcessor(CNetwork::DETECTORS);
}

DLL_EXPORT void __stdcall on_time_interval_complete()
{
    if (is_log_active && log_level == 2)
    {
        sprintf(out_buf, "Called %s function execution.", "ON_TIME_INTERVAL_COMPLETE()");
        OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
    }

    bool init = is_init == 0 ? false : true;
    if (!init && is_log_active && log_level == 2)
    {
        network->printDetectorsCount(); // print the total (cumulated on current simulation step) volume of detected vehicles
    }
}

DLL_EXPORT void __stdcall on_time_period_complete()
{
    if (is_log_active && log_level == 1)
    {
        sprintf(out_buf, "Called %s function execution.", "ON_TIME_PERIOD_COMPLETE()");
        OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
    }

    network->updateTimePeriodsDetectorsCount();

    if (is_log_active && log_level == 2)
    {
        network->printDetectorsCount();
    }
}

DLL_EXPORT void __stdcall on_net_signal_pre()
{
    if (is_log_active && log_level == 2)
    {
        sprintf(out_buf, "Called %s function execution.", "ON_NET_SIGNAL_PRE()");
        OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
    }

    // initialize the simulation time
    int sim_time = 0;

    // the corsim clock starts over after initialization
    // so the time at which initialization is over must be recorded
    if ((!is_init) && (prev_init))
    {
        end_of_init = prev_time + 1; // end of initialization
    }

    // adjust the time by adding the end of initialization
    sim_time = net_clock + end_of_init;

    // process any detector information
    network->processDetectors();

    // record whether the simulation has reached equilibrium or not, so the time at which initialization was over can be recorded
    prev_init = is_init;
    prev_time = sim_time;

    if (is_log_active && log_level == 2)
    {
        sprintf(out_buf , "\nCurrent total simulation time is %d sec.\n", sim_time);
        OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
    }
}

DLL_EXPORT void __stdcall on_simulation_complete()
{
    if (is_log_active && log_level == 2)
    {
        network->printDetectorsTransitions();
    }
    sprintf(out_buf, "Called %s function.\n", "ON_SIMULATION_COMPLETE()");
    OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
    // write output sensors file
    network->writeOutput();
}

DLL_EXPORT void __stdcall on_exit()
{
    sprintf(out_buf, "Called %s function.\n", "ON_EXIT()");
    OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
    // delete all objects that were created
    delete network;
}
