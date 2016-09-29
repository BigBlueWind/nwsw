//-------------------------------------------------------------------------------------------
/** @file LTE_doxygen_func_groups.c
 *  @brief LTE Doxygen Function Groups
 *  This File is used by Doxygen to form LTE functions groups and display them correctly
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// This File is used by Doxygen to form LTE functions groups and display them correctly
// Need not be included in any C projects
//----------------------------------------------------------------------------------------

//*****************************************************************************************************
//*****************************************************************************************************
// All group definitions
//-----------------------

/**
 * \defgroup group_lte_phy PHY Functions
 * @brief LTE PHY Layer Functions
 */

/**
 * \ingroup group_lte_phy
 * \defgroup group_lte_api MAC <-> PHY Functions
 * @brief MAC <-> PHY LTE Functions. These are the Mindspeed APIs to get to the Transcede&trade; Phy library
 */

/**
 * \ingroup group_lte_phy
 * \defgroup group_lte_rx_uplink RX Uplink
 */

/**
 * \ingroup group_lte_phy
 * \defgroup group_lte_tx_downlink Tx Downlink
 */

/**
 * \ingroup group_lte_rx_uplink
 * \defgroup group_lte_rx_uplink_fec_decoder FEC Decoder
 * @brief LTE Rx Uplink FEC Decoder Functions
 */

/**
 * \ingroup group_lte_rx_uplink
 * \defgroup group_lte_rx_uplink_demodulator Demodulator
 * @brief LTE Rx Uplink Demodulator Functions
 */

/**
 * \ingroup group_lte_tx_downlink
 * \defgroup group_lte_tx_downlink_fec_encoder FEC Encoder
 * @brief LTE Tx Downlink FEC Encoder Functions
 */

/**
 * \ingroup group_lte_tx_downlink
 * \defgroup group_lte_tx_downlink_modulator Modulator
 * @brief LTE Tx Downlink Modulator Functions
 */

/**
 * \ingroup group_lte_phy
 * \defgroup group_lte_utilities Utilities
 * @brief LTE Utility Functions
 */

/**
 * \ingroup group_lte_phy
 * \defgroup group_lte_tcb TCB Wrapper Functions
 * @brief LTE TCB (Task Control Block) Wrapper Functions
 *
 * <DIV style="text-align:justify;">
 * <B>Example TCB routine Code</B>
 *
 * As explained in the section (\ref phy_projects_a), there are 4 modes in which the PHY can be operated. This means that
 * the code would have to be compied differently for these 4 modes. This is acheived by using compile flags in the code (which helps
 * keeping all related items for each mode under the same area of the same file) for each mode. The TCB Routine seemed like a good
 * spot to place these flags as the TCB needs to be constructed differently for different modes. An example code is presented below
 * with an explaination. Each of the functions in the PHY code are structured just as shown.
 *
 * <BR/>
 *
 *
 * The example below is a TCB routine for a function that needs to be run on the CEVA. It takes in parameters <I>a, b, c, d</I> which
 * are all config parameters and array pointers <I>e, f</I> which are input and output arrays to the function. <I>c</I> is the input
 * length of array <I>e</I> and <I>d</I> is the input length of array <I>f</I>. Apart from this, it also takes in a parameter of type
 * PTCB (Pointer to a TCB) which is the dependency TCB. Hence this function depends on another function that needs to be executed
 * before this starts. <BR/><BR/><BR/>
 *
 * <I><B>Color Codes for the example code below:</B></I><BR/><BR/>
 * <SPAN class="cg">All Functions are in Dark Green</SPAN> <BR/>
 * <SPAN class="co">All Typedefs are in Orange</SPAN> <BR/>
 * <SPAN class="cb">All Variables are in Blue</SPAN> <BR/>
 * <SPAN class="clg">All Constants are in Light Green</SPAN> <BR/>
 * <SPAN class="cr">All # ifdefs, # else and # endif 's are in red</SPAN> <BR/>
 * <SPAN class="clb">All comments are in light blue</SPAN> <BR/>
 *
 * <BR/><BR/>
 * CODE_START
 *<B><Sco>PTCB</S> ExampleFunction_tcb(<Sco>S16</S> <Scbu>a</S>, <Sco>S16</S> <Scbu>b</S>, <Sco>S16</S> <Scbu>c</S>, <Sco>S16</S> <Scbu>d</S>, <Sco>S16</S> <Scbu>*e</S>, <Sco>S16</S> <Scbu>*f</S>, <Sco>PTCB</S> <Scbu>pTaskListDependencyTo</S>)</B>
 *<B>{</B>
 *     <Sco>PTCB</S> <Scbu>pTCB</S> = 0;
 *
 *<Scr># ifdef</S> <B>SCHEDULER</B>
 *     <Sco>S16</S> <Scbu>pContext</S>;
 *<Scr># endif</S>
 *
 *
 *<Scr># ifdef</S> <B>DEBUG_PRINT</B>
 *     <Scb>pTaskListDependencyTo</S> = <Scg>filePrint_tcb</S>(<B>"ExampleFunction_Input.txt"</B>, <Scb>e</S>, <Scb>c</S>, '<Sco>S16</S>', <B>"w"</B>, <Scb>pTaskListDependencyTo</S>);
 *<Scr># endif</S> <Sclb>// DEBUG_PRINT</S>
 *
 *
 *<Scr># ifndef</S> <B>SCHEDULER</B>     <Sclb>// Run C-Only</S>
 *
 *     <Scg>printf</S>(<B>"Executing Task [%3d]\n"</B>, <Sclg>TASKID_EXAMPLE_FUNCTION</S>);
 *     <Scg>ExampleFunction</S>(<Scb>a</S>, <Scb>b</S>, <Scb>c</S>, <Scb>d</S>, <Scb>e</S>, <Scb>f</S>);
 *
 *<Scr># else</S> <Sclb>// SCHEDULER</S>
 *
 *<Scr># ifdef</S> <B>USECEVA</B>
 *
 *     <Scb>pTCB</S> = <Scg>CreateTCB</S>(<Sclg>TASKID_EXAMPLE_FUNCTION</S>, <Sclg>RESOURCE_CEVA</S>, <Sco>NULL</S>, 4*<Scg>sizeof</S>(<Sco>S16</S>), <Sco>NULL</S>, <Sco>NULL</S>);
 *
 *<Scr># ifdef</S> <B>_WIN32</B>
 *     <Scg>RegARMTask</S>(<Sclg>TASKID_EXAMPLE_FUNCTION</S>, &<Scg>Example_Function_CEVA</S>);
 *<Scr># endif</S> <Sclb>// _WIN32</S>
 *
 *<Scr># else</S> <Sclb>// USECEVA</S>
 *     <Scb>pTCB</S> = <Scg>CreateTCB</S>(<Sclg>TASKID_EXAMPLE_FUNCTION</S>, <Sclg>RESOURCE_LARM</S>, <Sco>NULL</S>, 4*<Scg>sizeof</S>(<Sco>S16</S>), <Sco>NULL</S>, <Sco>NULL</S>);
 *     <Scg>RegARMTask</S>(<Sclg>TASKID_EXAMPLE_FUNCTION</S>, &<Scg>Example_Function_ARM</S>);
 *<Scr># endif</S> <Sclb>// USECEVA</S>
 *
 *     <Scb>pContext</S> = <Scb>pTCB</S>->ContextPtr;
 *     <Scb>pContext[0]</S> = <Scb>a</S>;
 *     <Scb>pContext[1]</S> = <Scb>b</S>;
 *     <Scb>pContext[2]</S> = <Scb>c</S>;
 *     <Scb>pContext[3]</S> = <Scb>d</S>;
 *
 *     <Scg>CreateTCBIOBuf</S>(<Scb>pTCB</S>, 2);
 *     <Scg>FillTCBIOBuf</S>(<Scb>pTCB</S>, (<Sco>U32</S>*) (<Scb>e</S>), <Scb>c</S> * <Scg>sizeof</S>(<Sco>S16</S>), <Sclg>IN_BUF</S>, <Sco>NULL</S>);
 *     <Scg>FillTCBIOBuf</S>(<Scb>pTCB</S>, (<Sco>U32</S>*) (<Scb>f</S>), <Scb>d</S> * <Scg>sizeof</S>(<Sco>S16</S>), <Sclg>OUT_BUF</S>, <Sco>NULL</S>);
 *
 *<Scr># ifdef</S> <B>LIST_BASED</B>
 *     <Scg>AddTCBToList</S>(<Scb>pTCB</S>, <Scb>pTaskListTx</S>, <Sco>NULL</S>);
 *     if (<Scb>pTaskListDependencyTo</S> != <Sco>NULL</S>)
 *          <Scg>AddDependency</S>(<Scb>pTCB</S>, 1, <Scb>pTaskListDependencyTo</S>);
 *
 *<Scr># ifdef</S> <B>_WIN32</B>
 *     <Scg>printf</S>(<B>"Added [%3d] to TaskList\n"</B>, <Sclg>TASKID_EXAMPLE_FUNCTION</S>);
 *<Scr># endif</S> <Sclb>// _WIN32</S>
 *
 *<Scr># else</S> <Sclb>// LIST_BASED</S>
 *
 *<Scr># ifdef</S> <B>_WIN32</B>
 *     <Scg>printf</S>(<B>"Example Function CEVA TCB\n"</B>);
 *<Scr># endif</S> <Sclb>//_WIN32</S>
 *
 *     <Scg>ExeTSchedSingle</S>(<Scb>pTCB</S>);
 *
 *<Scr># ifndef</S> _WIN32
 *     <Scg>printf</S>(<B>"Example Function CEVA w ticks %u\r\n"</B>, <Scb>pTCB</S>->ExecTicks);
 *<Scr># endif</S> <Sclb>// _WIN32</S>
 *
 *     <Scg>DestroyTCB</S>(<Scb>pTCB</S>);
 *     <Scb>pTCB</S> = 0;
 *
 *<Scr># endif</S> <Sclb>// LIST_BASED</S>
 *
 *<Scr># endif</S> <Sclb>// SCHEDULER</S>
 *
 *
 *<Scr># ifdef</S> <B>DEBUG_PRINT</B>
 *     <Scb>pTCB</S> = <Scg>filePrint_tcb</S>(<B>"ExampleFunction_Output.txt"</B>, <Scb>f</S>, <Scb>d</S>, '<Sco>S16</S>', <B>"w"</B>, <Scb>pTCB</S>);
 *<Scr># endif</S> <Sclb>// DEBUG_PRINT</S>
 *
 *
 *    <Scg>return</S> <Scb>pTCB</S>;
 *<B>}</B>
 * CODE_END
 *
 *
 * \section tcb_example_c 3.3  Explanation of example code
 *
 * TABLE_START
 * ------------------------------------------------------------------------------------------------------------------------------
 * |Line Numbers  |Explaination                                                                                                 |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |1             |Function Protoype. The function takes in 4 config parameters <B>a, b, c, d</B> and 2 input/output array      |
 * |              |pointers <B>e & f</B>, whose lengths are c and d respectively. It has has a PTCB <B>pTaskListDependencyTo</B>|
 * |              |which is used to add dependencies.                                                                           |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |3             |Declaration of a PTCB. The CreateTCB API would return back a PTCB. This variable is used to store the TCB    |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |5-7           |This is used to set the context pointer of the PTCB returned. It is not needed when running the C-Only mode  |
 * |              |(2.1.1). Hence the Flag <B>SCHEDULER</B> is used to check for this condition this out.                       |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |10-11         |This section creates a file and writes the input <B>e</B> to the file: <B>ExampleFunction_Input.txt</B>.     |
 * |              |This function call itself would create TCBs (if required) when running under mode 2.1.2. Its structure is    |
 * |              |very similar to this example function. It is not called in ode 2.1.3 (as the device cannot do fprintf's. The |
 * |              |flag <B>DEBUG_PRINT</B> is used to check for this condition                                                  |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |15-20         |When running mode 2.1.1 (C-Only Mode), there is no need to create TCBs. Hence the "ExampleFunction" is       |
 * |              |directly called by passing the parameters. The <B>SCHEDULER</B> flag is is used to check for the mode        |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |20-72         |If <B>SCHEDULER</B> is defined, then either mode 2.1.2 or 2.1.3 could be run. All lines in between here are  |
 * |              |for these modes                                                                                              |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |22-30         |This task could either be run in the CEVA or the ARM. If <B>USECEVA</B> is defined, then it would run on     |
 * |              |the CEVA                                                                                                     |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |24            |A TCB is created with <B>TASKID_EXAMPLE_FUNCTION</B> on </B>RESOURCE_CEVA</B> with 4 context locations to    |
 * |              |store variables <B>a, b, c and d</B>.                                                                        |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |26-28         |When running under the CEVA SIMULATOR mode (2.1.2), we need to register this ID with a Dummy ARM function    |
 * |              |<B>Example_Function_CEVA</B>, which would handle the calling of the actual function in the CEVA simulator    |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |30-33         |If <B>USECEVA</B> is not defined, then this task would be run on the ARM. In this case, a TCB is created in  |
 * |              |the exact same manner but with <B>RESOURCE_LARM</B> and a dummy function <B>Example_Function_ARM</B> is      |
 * |              |registered which would handle dispatching the task to the ARM                                                |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |35-39         |All of the config parameters <B>a, b, c & d</B> are passed through the TCB to the function by placing them   |
 * |              |on the context locations allocated when creating the TCB                                                     |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |41-43         |The address locations of <B>e & f</B> are passed to the resource by creating 2 IOBUFs on the TCB and Filling |
 * |              |them up as shown. It copies data from DDR to CRAM if necessary in the 2.1.3 mode. Once this step is done, the|
 * |              |<B>TCB is created</B>.                                                                                       |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |45-54         |This section adds the created TCBs to an existing List <B>pTaskListTx</B>. It adds the dependency to the list|
 * |              |if needed as well. The flag <B>LIST_BASED</B> is used to check for this                                      |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |46            |Adds TCB to list                                                                                             |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |47-48         |Adds dependency to this TCB with the PTCB passed into this function <B>pTaskListDependencyTo</B>.            |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |54-70         |If the TCB needs to be executed on the spot and not added to the List, then this section takes care of that  |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |60            |Executes the TCB as it is                                                                                    |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |66-68         |This sections does all the cleanup after running the TCB. Update Profiling, updates a profile variable with  |
 * |              |the number of cycles it took to run the task. This is passed back from the RESOURCE through the TCB. The     |
 * |              |DestroyTCB task, deallocates memory from the TCB and cleans up the space                                     |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |75-77         |This is similar to lines 10-12. The output <B>f</B> is stoed into file: <B>ExampleFunction_Output.txt</B>    |
 * ------------------------------------------------------------------------------------------------------------------------------
 * |17, 51, 57, 63|These lines are used as feedback to the screen and updating the processing of the function under different   |
 * |              |modes                                                                                                        |
 * ------------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 *
 * <BR/><BR/>
 * </DIV>
 */



/**
 * \ingroup group_lte_phy
 * \defgroup group_lte_phy_control_rt Real Time Thread Functions (Phy Control)
 * @brief This sections describe the functions that are used for real-time execution of the PHY on device.
 *           These functions use the 4GMX Thread APIs to dispatch PHY ARM functions to various ARMs on device
 *
*/

/**
 * \ingroup group_lte_phy_control_rt
 * \defgroup group_lte_phy_control_rt_tx BS Transmitter Thread Functions
 * @brief Real Time Bs Tx (DL) Thread Functions
 */

/**
 * \ingroup group_lte_phy_control_rt_tx
 * \defgroup group_lte_phy_control_rt_tx_setup MAC API Parse Thread Functions
 * @brief Real Time Bs Tx (DL) Thread MAC API Parse Functions
 */

/**
 * \ingroup group_lte_phy_control_rt_tx
 * \defgroup group_lte_phy_control_rt_tx_sdu SDU Processing (FEC Encoder) Thread Functions
 * @brief Real Time Bs Tx (DL) SDU Processing (FEC Encoder) Thread Functions
 */

/**
 * \ingroup group_lte_phy_control_rt_tx
 * \defgroup group_lte_phy_control_rt_tx_listrun List Processing (Modulator) Thread Functions
 * @brief Real Time Bs Tx (DL) List Processing (Modulator) Thread Functions
 */

/**
 * \ingroup group_lte_phy_control_rt_tx
 * \defgroup group_lte_phy_control_rt_tx_update Setup / Update Thread Functions
 * @brief Real Time Bs Tx (DL) List Processing (Modulator) Thread Functions
 */


/**
 * \ingroup group_lte_phy_control_rt
 * \defgroup group_lte_phy_control_rt_rx BS Receiver Thread Functions
 * @brief Real Time Bs Rx (UL) Thread Functions
 */

/**
 * \ingroup group_lte_phy_control_rt_rx
 * \defgroup group_lte_phy_control_rt_rx_setup MAC API Parse Thread Functions
 * @brief Real Time Bs Rx (UL) Thread MAC API Parse Functions
 */

/**
 * \ingroup group_lte_phy_control_rt_rx
 * \defgroup group_lte_phy_control_rt_rx_sdu SDU Processing (FEC Decoder) Thread Functions
 * @brief Real Time Bs Rx (UL) SDU Processing (FEC Encoder) Thread Functions
 */

/**
 * \ingroup group_lte_phy_control_rt_rx
 * \defgroup group_lte_phy_control_rt_rx_listrun List Processing (De-Modulator) Thread Functions
 * @brief Real Time Bs Rx (UL) List Processing (Modulator) Thread Functions
 */

/**
 * \ingroup group_lte_phy_control_rt_rx
 * \defgroup group_lte_phy_control_rt_rx_prach PRACH Thread Functions
 * @brief Real Time Bs Rx (UL) PRACH Thread Functions
 */

/**
 * \ingroup group_lte_phy_control_rt_rx
 * \defgroup group_lte_phy_control_rt_rx_srs SRS Thread Functions
 * @brief Real Time Bs Rx (UL) SRS Thread Functions
 */


/**
 * \ingroup group_lte_phy_control_rt
 * \defgroup group_lte_phy_control_rt_common BS Thread Functions (Common)
 * @brief Real Time Bs Common Thread Functions
 */

/**
 * \ingroup group_lte_phy_control_rt_common
 * \defgroup group_lte_phy_control_rt_mac_phy \MAC <-> PHY Thread Functions
 * @brief Real Time MAC <-> PHY Thread Functions
 */

/**
 * \ingroup group_lte_phy_control_rt_common
 * \defgroup group_lte_phy_control_rt_data_interface Data Interface Functions
 * @brief Real Time Data Interface Functions
 */

/**
 * \ingroup group_lte_phy_control_rt
 * \defgroup group_lte_phy_control_rt_test BS Real Time Testing Functions
 * @brief Real Time Bs Real Time Testing Functions
 */

/**
 * \ingroup group_lte_phy_control_rt_test
 * \defgroup group_lte_phy_control_rt_api_player API Player Functions
 * @brief Real Time API Player Functions
 */


//*****************************************************************************************************



//*****************************************************************************************************
// 4GMX Functions
//---------------

/**
 *
 * \defgroup group_lte_4gmx 4GMX (Lower SubSystem Operating System) Functions
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Function
 *
 * <DIV style="text-align:justify;">
 * The Transcede&trade; 4000 Lower Sub-System <I><B>(LSS)</B></I> offers several HW resources that the application
 * programmer can utilize; these include ARM, CEVA, FEC, and MAP. A typical lower subsystem application is
 * written in the C language and has functions that are written to take advantage of a specific HW resource.
 * It is the responsibility of application programmer to decide on which functions are better suited for a
 * certain HW resource based on its processing properties. For example, the FEC processor is very suited to
 * run FEC encoding and decoding functions while the CEVA processor is suited for running general DSP algorithms.
 *
 * <BR/>
 *
 * From an application point-of-view, the lower subsystem consists of (as shown in the Figure below): <BR/>
 * CODE_START
 * An API Parser that is application specific (for example Intel API parser for WiMax)
 * An application-specific Data Analysis and Partitioning (DAP) layer
 * Transcede&trade; 4000 Lower Sub-System Transaction Executve (TrEx / REX) Layer
 * System Services
 * Hardware Abstraction Layer (HAL) / System Drivers
 * <Scr>An Operating System (4GMX)</S>
 * CODE_END
 *
 * <BR/>
 * <img src="Lss.png"/>
 * <BR/>
 *
 * The 4GMX operating system is designed to provide control threads based on a single-stack mode, inter-
 * CPU functionality, driver controls and APIs to control the data objects used in the applications.
 * The 4GMX system provides several OS primitives which are grouped into modules shown above. Each primitive
 * can be used both in the thread execution context and in the ISR execution context. These APIs are
 * descrbed in seperate sections. For more information on the 4GMX operating system, please refer to the documentation section.
 *
 * <BR/><BR/>
 * </DIV>
 */

/**
 * \ingroup group_lte_4gmx
 * \defgroup group_lte_4gmx_kernel Kernel APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Kernel APIs
 */

/**
 * \ingroup group_lte_4gmx_kernel
 * \defgroup group_lte_4gmx_kernel_mmu Kernel MMU APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Kernel MMU APIs
 */



/**
 * \ingroup group_lte_4gmx
 * \defgroup group_lte_4gmx_thread Thread APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Thread APIs
 */

/**
 * \ingroup group_lte_4gmx
 * \defgroup group_lte_4gmx_event Event APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Event APIs
 */

/**
 * \ingroup group_lte_4gmx
 * \defgroup group_lte_4gmx_heap Heap APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Heap APIs
 */

/**
 * \ingroup group_lte_4gmx
 * \defgroup group_lte_4gmx_rapid_heap Rapid Heap APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Rapid Heap APIs
 */

/**
 * \ingroup group_lte_4gmx
 * \defgroup group_lte_4gmx_partition Partition APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Partition APIs
 */

/**
 * \ingroup group_lte_4gmx
 * \defgroup group_lte_4gmx_inter_cpu Inter-CPU APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Inter-CPU APIs
 */

/**
 * \ingroup group_lte_4gmx
 * \defgroup group_lte_4gmx_list List APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) List APIs
 */

/**
 * \ingroup group_lte_4gmx
 * \defgroup group_lte_4gmx_queue Queue APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Queue APIs
 */

/**
 * \ingroup group_lte_4gmx
 * \defgroup group_lte_4gmx_service Service APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Service APIs
 */

/**
 * \ingroup group_lte_4gmx
 * \defgroup group_lte_4gmx_debug Debug APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Debug APIs
 */

/**
 * \ingroup group_lte_4gmx
 * \defgroup group_lte_4gmx_timer Timer APIs
 * @brief Mindspeed 4GMX (Lower SubSystem Operating System) Timer APIs
 */
//*****************************************************************************************************


//*****************************************************************************************************
// Syscore Functions
//------------------

/**
 *
 * \defgroup group_lte_syscore Mindspeed Transcede 4000 Syscore Package
 * @brief Mindspeed Transcede 4000 Syscore Package
 *
 * <DIV style="text-align:justify;">
 * The Transcede&trade; 4000 Lower Sub-System <I><B>(LSS)</B></I> offers several HW resources that the application
 * programmer can utilize; these include ARM, CEVA, FEC, and MAP. A typical lower subsystem application is
 * written in the C language and has functions that are written to take advantage of a specific HW resource.
 * It is the responsibility of application programmer to decide on which functions are better suited for a
 * certain HW resource based on its processing properties. For example, the FEC processor is very suited to
 * run FEC encoding and decoding functions while the CEVA processor is suited for running general DSP algorithms.
 *
 * <BR/>
 *
 * From an application point-of-view, the lower subsystem consists of (as shown in the Figure below): <BR/>
 * CODE_START
 * An API Parser that is application specific (for example Intel API parser for WiMax)
 * An application-specific Data Analysis and Partitioning (DAP) layer
 * <Scr>Transcede&trade; 4000 Lower Sub-System Transaction Executve (TrEx / REX) Layer</S>
 * <Scr>System Services</S>
 * <Scr>Hardware Abstraction Layer (HAL) / System Drivers</S>
 * An Operating System (4GMX)
 * CODE_END
 *
 * <BR/>
 * <img src="Lss.png"/>
 * <BR/>
 *
 * This section deals with the System Space from diagram aboe and the various services / drivers that Mindspeed
 * provides a programmer to use with the device. These are in the form of APIs that have been verified by the
 * Mindspeed Development Team to be working with the Transcede device.
 *
 * <BR/><BR/>
 * </DIV>
 */


//*****************************************************************************************************
// Syscore Services
//-----------------

/**
 * \ingroup group_lte_syscore
 * \defgroup group_lte_services Syscore Services
 * @brief Syscore Services
 *
 * <DIV style="text-align:justify;">
 * This section deals with the various services that Mindspeed provides a programmer that helps in developing
 * and debugging his/her software running on device. These are in the form of APIs that have been verified by
 * the Mindspeed Development Team to be working with the Transcede device.
 *
 * <BR/><BR/>
 * </DIV>
 */

/**
 * \ingroup group_lte_services
 * \defgroup group_lte_services_scheduler TCB Scheduler Functions
 * @brief TCB Scheduler Functions
 *
 * <DIV style="text-align:justify;">
 * The LSS TrEx / REX layer accepts a task list consisting of multiple TCBs and is responsible for posting (or queuing)
 * a TCB to the proper HAL driver at the correct time. Therefore, the TrEx's / REX's job is to decide <B>what</B> and
 * <B>when</B> a TCB should be executed. In a case of multiple resources of the same type, (as in the CEVA DSP
 * processors), TrEx / REX may determine the DSP ID on which a task runs, or rely on the HAL driver to make that assignment.
 *
 * <BR/>
 *
 * TCB stands for <B>T</B>ransaction <B>C</B>ontrol <B>B</B>lock. Each of the DSP routines in the PHY code would be scheduled
 * to the individual processors by creating TCBs. These Blocks are created using TCB APIs (<B>A</B>pplication <B>P</B>rogrammers
 * <B>I</B>nterface) which are used inside of the TCB routines. Hence each tasks has its own TCB routine.
 *
 * <BR/><BR/>
 *
 * Mindspeed provides 2 different scheduler algorithms today
 * - TrEx (Transaction Executive)
 * - REX  (Rapid Executive)
 *
 * These schedulers and their APIs are explained in corresponding sections
 *
 * <BR/><BR/>
 * </DIV>
 */

/**
 * \ingroup group_lte_services_scheduler
 * \defgroup group_lte_tcbscheduler TrEx (Transaction Executive) Functions
 * @brief Mindspeed Transaction Executive (TrEx) Functions
 *
 * <DIV style="text-align:justify;">
 * The TrEx was the v1.0 scheduler that Mindspeed provided to customers to use along with the Transcede device. It has very complicated
 * functionality and supports a lot of features to make the life of a programmer very easy. It has built-in SMARTs to decide which path
 * is optimal in terms of scheduling. Obviously this comes at the expense of ARM MIPs.
 *
 * <BR/><BR/>
 *
 * <B>Brief description of TCB APIs</B><BR/>
 * This section breifly touches on the various TrEx APIs used inside the TCB routines. For a more detailed description of these APIs, please
 * refer to the <I><B>Lower Sub-System Transaction Executive (TrEx) section of the Programmer's Guide.</B></I><BR/><BR/>
 *
 * TABLE_START
 * --------------------------------------------------------------------------------------------------------------------------
 * |API                 |Purpose                                                                                            |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref CreateTCB           |This function creates (allocates memory for) a TCB structure and initializes its elements with     |
 * |                    |default values, and with parameters passed into this API.                                          |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref DestroyTCB          |This function destroys (de-allocates from memory) a previously created TCB                         |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref CreateTCBIOBuf      |This function creates (allocates in memory) an IO Buffer Descriptor used for passing I/O parameters|
 * |                    |from/to a C function. An IO Buffer descriptor is not in itself a container for the IO Buffer but   |
 * |                    |rather it captures where the buffer is located and its properties. The actual IO Buffer is         |
 * |                    |allocated separately outside this API.                                                             |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref FillTCBIOBuf        |This function is called as many times as the number of IO buffer descriptors (NumDesc) with the    |
 * |                    |same TCB pointer so that a C-function's input and output parameters all get converted to the TCB   |
 * |                    |structure.                                                                                         |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref extractTCBIOBuf     |This API is used to return the IO buffer pointer from a previously constructed TCB. This API is    |
 * |                    |normally used inside task.                                                                         |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref CreateTaskList      |This function is called to create and initialize an instance of a task list.                       |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref DestroyTaskList     |This funtion destroys (de-allocates from memory) a previously created list                         |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref AddTCBToList        |Use this API to add an existing TCB to a Task List. The newly added TCB is chained in a linked-list|
 * |                    |structure to the other TCBs in this task list.                                                     |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref AddDependency       |The Dependency List captures the inter-dependency among the different TCBs. This API is used to    |
 * |                    |this dependency between different TCBs.                                                            |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref ExeTSchedList       |The function calls TrEx by passing any TCB from a pre-constructed Task List. TrEx in turn          |
 * |                    |dispatches the tasks (TCBs) in the list starting with first TCB that has met its execution         |
 * |                    |pre-conditions.                                                                                    |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref ExeTSchedSingle     |This API provides a means to dispatch a single TCB and hence is used mainly for testing. The TCB   |
 * |                    |does not need to be part of a Task List. This call is blocking and will return once the TCB is     |
 * |                    |launched and has completed execution.                                                              |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref RegARMTask          |If a TCB (task) uses the ARM as its target hardware resource (TCB with the resource type           |
 * |                    |RESOURCE_LOWERARM), then the application has to register a function to be called by TrEx when a    |
 * |                    |task with (taskid) is ready to execute. This API is called to register such a function pointer with|
 * |                    |TrEx.                                                                                              |
 * --------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 * <BR/>
 *
 * </DIV>
 */


/**
 * \ingroup group_lte_services_scheduler
 * \defgroup group_lte_rexscheduler REX (Rapid Executive) Functions
 * @brief Mindspeed Rapid Executive (REX) Functions
 *
 * <DIV style="text-align:justify;">
 * The REX was a more recent addition to the Mindspeed Library. As the name says, it is supposed to a Rapid Transaction Executive.
 * Some of the SMARTs from the TrEx were stripped down to make it run faster and to acheive RealTime processing. It is backward
 * compaible with the TrEx (meaning the APIs remain intact). A compile switch in code is used to select between the 2 scehdulers.
 *
 * <BR/><BR/>
 *
 * <B>Brief description of TCB APIs</B><BR/>
 * This section breifly touches on the various REX APIs used inside the TCB routines. <BR/><BR/>
 *
 * TABLE_START
 * --------------------------------------------------------------------------------------------------------------------------
 * |API                 |Purpose                                                                                            |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref CreateTCB           |This function creates (allocates memory for) a TCB structure and initializes its elements with     |
 * |                    |default values, and with parameters passed into this API.                                          |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref DestroyTCB          |This function destroys (de-allocates from memory) a previously created TCB                         |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref CreateTCBIOBuf      |This function creates (allocates in memory) an IO Buffer Descriptor used for passing I/O parameters|
 * |                    |from/to a C function. An IO Buffer descriptor is not in itself a container for the IO Buffer but   |
 * |                    |rather it captures where the buffer is located and its properties. The actual IO Buffer is         |
 * |                    |allocated separately outside this API.                                                             |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref FillTCBIOBuf        |This function is called as many times as the number of IO buffer descriptors (NumDesc) with the    |
 * |                    |same TCB pointer so that a C-function's input and output parameters all get converted to the TCB   |
 * |                    |structure.                                                                                         |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref extractTCBIOBuf     |This API is used to return the IO buffer pointer from a previously constructed TCB. This API is    |
 * |                    |normally used inside task.                                                                         |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref CreateTaskList      |This function is called to create and initialize an instance of a task list.                       |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref DestroyTaskList     |This funtion destroys (de-allocates from memory) a previously created list                         |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref AddTCBToList        |Use this API to add an existing TCB to a Task List. The newly added TCB is chained in a linked-list|
 * |                    |structure to the other TCBs in this task list.                                                     |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref AddDependency       |The Dependency List captures the inter-dependency among the different TCBs. This API is used to    |
 * |                    |this dependency between different TCBs.                                                            |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref ExeTSchedList       |The function calls TrEx by passing any TCB from a pre-constructed Task List. TrEx in turn          |
 * |                    |dispatches the tasks (TCBs) in the list starting with first TCB that has met its execution         |
 * |                    |pre-conditions.                                                                                    |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref ExeTSchedSingle     |This API provides a means to dispatch a single TCB and hence is used mainly for testing. The TCB   |
 * |                    |does not need to be part of a Task List. This call is blocking and will return once the TCB is     |
 * |                    |launched and has completed execution.                                                              |
 * --------------------------------------------------------------------------------------------------------------------------
 * |\ref RegARMTask          |If a TCB (task) uses the ARM as its target hardware resource (TCB with the resource type           |
 * |                    |RESOURCE_LOWERARM), then the application has to register a function to be called by TrEx when a    |
 * |                    |task with (taskid) is ready to execute. This API is called to register such a function pointer with|
 * |                    |TrEx.                                                                                              |
 * --------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 * <BR/>
 *
 * </DIV>
 */

/**
 * \ingroup group_lte_services
 * \defgroup group_lte_logging Logging Services
 * @brief Mindspeed Transaction Executive (TrEx/REX) Profiler and System Debug Trace Logger
 *
 * <DIV style="text-align:justify;">
 * This section deals with the various logging features that are provided by Mindspeed to
 * help programmer debug code easily.
 *
 * <BR/><BR/>
 * </DIV>
 */



/**
 * \ingroup group_lte_logging
 * \defgroup group_lte_mlog MLOG (Mindspeed Log) Functions
 * @brief Mindspeed Transaction Executive (TrEx/REX) Profiler and System Debug Trace Logger
 *
 * <DIV style="text-align:justify;">
 * This sections defines the API to use the Mindspeed TrEx / REX Profiler and System Debug Trace Logger.
 * In short, we call it MLOG (Mindspeed Logger).
 * When running the code on the Device, it is very useful to be able to log all information about
 * the system at run time so that it can be analyzed at a later stage to aid in optimizing /
 * debugging the system efficiently and correctly. When using a chip which has a suite of processors
 * it becomes very hard to track what was run where and when. To help aid programmers with this issue
 * Mindspeed provides a very rich suite of fucntions, some which are automatically called by our TrEx / REX
 * <I><B>(Transaction Executive)</B></I>, and others which can be used by the programmer to log in
 * any important and useful information that will aid in his/her debugging and optimization efforts.
 * These functions are very tightly coupled with the <I><B>4GMX Operating system</B></I> and the
 * <I><B>Transaction Executive</B></I>. Programmers within Mindspeed use these functions all the time
 * to debug real time issues.
 *
 * <BR/>
 *
 * The idea behind the MLOG is very simple. Whenever a task is dispatched by the TrEx / REX to a specific
 * module on chip, it logs in the start time of execution of this task, associated IDs to recoginize the
 * module and other such information using these APIs to a predetermined space (allocated by user using API)
 * in external DDR memory. This information is later on taken out of device and moved to a PC to analyze the
 * information using Mindspeed's Offline Scheduler Tool, which we call TTBOX <I><B>Transcede&trade; Tool Box</B></I>.
 * This tool is a very powerful utility in the terms that it dsiplay valuable statistics of the log captured
 * and displays information such as processor utilization, critical path functions, dependencies, etc.
 *
 * <BR/>
 *
 * There is a global clock that is readable by the ARM / CEVA processors which currently is set to run at 150MHz.
 * Whenever the MLOG is asked to log in a task, it reads the status of this register and stores this value into memory.
 * So there is read to this clock during the start and then another read during the end. The Offline TTBox tool, which
 * recognizes what is what, takes the difference, plots graphs and displays vital statistics. For more information on MLOG
 * and TTBox, please refer to the Transcede&trade; Programming Guide.
 *
 * <BR/>
 *
 * There are a suite of functions that we provide as part of the MLOG deliverable. Here is a snap shot of the functions.
 * A description of each and it's I/O param details are shown when you click on the links in the table below:
 *
 * <BR/>
 *
 * TABLE_START
 * --------------------------------------------------------------------
 * |Function Name              |Automatically called? |Called when?   |
 * --------------------------------------------------------------------
 * |\ref MLogOpen              |TrEx / REX            |Init           |
 * --------------------------------------------------------------------
 * |\ref MLogClose             |TrEx / REX            |Init           |
 * --------------------------------------------------------------------
 * |\ref MLogTCBList           |TrEx / REX            |Init           |
 * --------------------------------------------------------------------
 * |\ref MLogTCBListStart      |TrEx / REX            |RunTime        |
 * --------------------------------------------------------------------
 * |\ref MLogTCBListStop       |TrEx / REX            |RunTime        |
 * --------------------------------------------------------------------
 * |\ref MLogTCB               |TrEx / REX            |Init           |
 * --------------------------------------------------------------------
 * |\ref MLogEXEStart          |TrEx / REX            |RunTime        |
 * --------------------------------------------------------------------
 * |\ref MLogEXEFinish         |TrEx / REX            |RunTime        |
 * --------------------------------------------------------------------
 * |\ref MLogEXEFinish2        |TrEx / REX            |RunTime        |
 * --------------------------------------------------------------------
 * |\ref MLogSubTask           |TrEx / REX            |RunTime        |
 * --------------------------------------------------------------------
 * |\ref MLogDepends           |TrEx / User Defined   |Init           |
 * --------------------------------------------------------------------
 * |\ref MLogFREQ              |TrEx / REX            |Init           |
 * --------------------------------------------------------------------
 * |\ref MLogEnableRC          |4GMX                  |Init           |
 * --------------------------------------------------------------------
 * |\ref MLogResourceCondition |4GMX                  |RunTime        |
 * --------------------------------------------------------------------
 * |\ref MLogTask              |User Defined          |RunTime        |
 * --------------------------------------------------------------------
 * |\ref MLogMark              |User Defined          |RunTime        |
 * --------------------------------------------------------------------
 * |\ref MLogPrint             |User Defined          |Cleanup        |
 * --------------------------------------------------------------------
 * |\ref MLogGetFileSize       |User Defined          |Cleanup        |
 * --------------------------------------------------------------------
 * |\ref MLogGetFileLocation   |User Defined          |Cleanup        |
 * --------------------------------------------------------------------
 * TABLE_END
 *
 * <BR/><BR/>
 *
 * By default Mindspeed's release has the MLOG feature enabled and can be easily turned off by undefining certain compile switch
 * and recompiling the project. The compile flags are defined in the mlog.h file included in the release.
 *
 * <BR/>
 *
 * TABLE_START
 * ------------------------------------------------------------------------------------------------------------------------
 * |Compile Flag        |Description                                                                                      |
 * ------------------------------------------------------------------------------------------------------------------------
 * |MLOG_SUBTASK_ENABLE |This flag turns on the sub-task logging on sub-tasks within a Ceva task                          |
 * ------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 *
 *
 * <BR/><BR/>
 * </DIV>
 */


//*****************************************************************************************************
// Syscore Drivers
//----------------

/**
 * \ingroup group_lte_syscore
 * \defgroup group_lte_hal Syscore Drivers
 * @brief LTE HAL (Hardware Abstraction Layer) Functions
 *
 * <DIV style="text-align:justify;">
 * This section deals with the various Hardware drivers that Mindspeed provides programmers to use with the
 * Transcede device. Each type of HW resource has its own HAL driver, for example, all the CEVA DSPs share one CEVA driver.
 * Each driver has a queue to accept incoming TCBs from the scheduler and launches the task on a specific resource when
 * available. When the task is completed (i.e. TCB is done), the HAL driver informs TrEx / REX of a TCB's completion.
 *
 * <BR/>
 *
 * All the driver API are described under each of the Rerouce Modules under this section. For a more detailed
 * description of these APIs, please refer to the Transcede&trade; Programming Guide.
 *
 * <BR/><BR/>
 * </DIV>
 */

/**
 * \ingroup group_lte_hal
 * \defgroup group_lte_hal_dsp DSP (CEVA) Driver Functions
 * @brief DSP (CEVA) Driver Functions
 *
 * <DIV style="text-align:justify;">
 * This sections defines the APIs for the DSP Driver provided as part of Mindspeed's reference library. The Transcede&trade;
 * 4000 processor has 10 CEVA DSPs which we refer to the CEVA farm. These are controllled in the reference PHY solution
 * using the TrEx / REX subsystem. These drivers are very tightly coupled with the 4GMX Operating system and TrEx / REX subsystem.
 * <BR/><BR/>
 *
 *
 * The APIs that are part of the DSP drivers are listed in the table below and I/O params are explained in the Function
 * Documentation section.
 * <BR/><BR/>
 *
 * TABLE_START
 * ---------------------------------------------------------
 * |Item               | Functions                         |
 * ---------------------------------------------------------
 * |Initialization     | \ref DspDrvInit                   |
 * ---------------------------------------------------------
 * |Boot-up            | \ref DspDrvBoot                   |
 * |                   | \ref DspDrvBist                   |
 * |                   | \ref DspDrvBistDmaUtil            |
 * ---------------------------------------------------------
 * |Dispatch / Run     | \ref DspDrvRun                    |
 * |                   | \ref DspDrvRunBlocking            |
 * ---------------------------------------------------------
 * |Interrupt routines | \ref DspDrvGetCevaIRQStatus       |
 * |                   | \ref DspDrvCheckCevaIrqStatus     |
 * |                   | \ref DspDrvIrqHandler             |
 * |                   | \ref DSPDrvSetCallBack            |
 * ---------------------------------------------------------
 * |Shutdown           | \ref DspDrvShutdown               |
 * ---------------------------------------------------------
 * |Debug              | \ref DspDrvDump                   |
 * ---------------------------------------------------------
 * TABLE_END
 *
 *
 * <BR/><BR/>
 * </DIV>
 */

/**
 * \ingroup group_lte_hal
 * \defgroup group_lte_hal_map MAP (Mindspeed Advanced Processor) Driver Functions
 * @brief MAP (Mindspeed Advanced Processor) Driver Functions
 *
 * <DIV style="text-align:justify;">
 * MAP stands for <I><B>M</B></I>indspeed <I><B>A</B></I>dvanced <I><B>P</B></I>rocessor. There are 10 of these
 * processors in the Transcede&trade; 4000 product. This processor is referred to by several different names within Mindspeed.
 * Some of which are:
 * - Filter Processor (FP)
 * - Mindspeed Accelerator Processor
 * - Filter Co-Processor
 * - FFT Hardware Engine
 *
 * All of these refer to the same processor on device. The Filter Processor is an Application Specific DSP
 * <I><B>(ASDSP)</B></I> for FFT, DCT and filtering applications. The processor has a 4x48-bit wide data path with 4 MAC
 * units of width 16x24 bits. The processor is optimized for MAC and butterfly calculations. The processor has tightly
 * coupled memories internally with a bandwidth of 352 bits per clock cycle. At a clock frequency of 750 MHz the
 * bandwidth to the memories will be <B>264 Gbit/s</B>. The external interface to the DMA, read and write has a bandwidth of
 * 2*64 bits per clock cycle. A summary of some typical applications, features and the instruction set are listed below. <BR/><BR/>
 *
 *  <B style="font-size:15px;">1. Applications: </B><BR/>
 *  -# FFT and IFFT
 *  -# DFT and IDFT
 *  -# FIR/IIR/WDF Filters
 *  -# Adaptive Filters
 *  -# Echo Cancellers
 *  -# DCT and IDCT
 *  -# Rake Receiver
 *
 *  <B style="font-size:15px;">2. Features: </B><BR/>
 * - 4 MAC
 * - 1 Cycle Complex Multiply
 * - 4 48-bit Accumulators with Add/Sub and Shift
 * - Integer and Fractional Data Formats
 * - Overflow Counter
 * - Peak Detector
 * - Circular Buffer addressing
 * - Segment and Bank Addressing Mode.
 * - Byte Addressing Mode.
 * - Instruction Loop Cache for low power performance
 * - Function Call, direct or pointer
 * - High Bandwidth Internal Memory Banks
 * - DMA with Bank Switch Controller interface
 * - Scratchpad registers with IO control Flags
 * - Bi-directional Event Flag Register
 * - 8 Independent Interrupt Lines
 * - 24-bit Debug Counter
 *
 * The processor is programmable in a C-like assembly language. The processor has 13 different fine-grained VLIW
 * instructions and a total of 120 different operations. <BR/><BR/>
 *
 *  <B style="font-size:15px;">3. Instructions: </B><BR/>
 * - 1 Control Instruction
 * - 1 Address Load
 * - 1 Circular Buffer Instruction
 * - 2 Complex Multiply and Add/Sub.
 * - 4 Multiply Accumulate, Add/Sub.
 * - 1 Multiply Accumulate, Add/Sub/Shift
 * - 1 Immediate load and Shift
 * - 1 Byte Address Instruction
 * - 1 IO instruction
 *
 * The processor has a program memory of 512 words of 160-bits each and a data memory of 80 KBs spread across 16-bit
 * and 24-bit memroy banks. The program is loaded via the APB bus by the controller processor (ARM). For a more detailed
 * description of he assembly langueage and hardware architecture, please refer to the FP Reference Architecture and
 * Programming Guide. <BR/><BR/>
 *
 * <B style="font-size:15px;">4. FPDMA:</B><BR/><BR/>
 * The processor is present on the device in 2 pools of 5 MAPs each. Each pool as its own DMA Engine which we refer to
 * as the <I><B>FPDMA</B></I> or <I><B>MAPDMA</B></I>. This engine is responsible for data movement between the MAPs and the
 * external memory (CRAM or DDR). This unit has a very special hardware structure described below:
 * - Each DMA is like a hardware dispatcher
 * - It can take a hardware descriptor (special C-structure) that describes the operation of the MAP for the current task
 * - The hardware descriptor has information burried in it like
 *    - I/O pointers in memory to read from / write to and teir lengths
 *    - Interrupt format
 *    - Task to execute in MAP
 * - There could be multiple descriptors like this chained together into 1 giant descriptor. So in a way, these descriptors are like
 *   a linked-list of tasks to run on a specific MAP or group of MAPs.
 * - Because of this, if a set of tasks are described using these descriptors (1 per task) and chained together, the FPDMA / MAP will
 *   run the chain to completion before returning back to the ARM with an interrupt.
 * - This is very important because the number of ARM IRQs for the MAP considerably drop with this model.
 *
 * There are 2 modes that the FPDMA can operate under:
 * - <B>POOL Mode</B>: In this mode, the FPDMA decides which MAP to run the task from the descriptor in. Once completed, the FPDMA interrupts
 *   the ARM. When there is a chain of descriptors, the FPDMA runs the chain in as many MAPs as it can
 * - <B>FEXP Mode</B>: In this mode, the FPDMA is told which task to run in which MAP by the descriptor. Once completed, the MAP directly
 *   the ARM interrupts. In this mode, the number of ARM IRQs because of the MAP are high.
 *
 * <BR/>
 *
 * <B style="font-size:15px;">5. Transcede&trade; Tasks run on MAP:</B><BR/><BR/>
 * In the Transcede 4000 device, Mindspeed's reference PHY run <B>FFT, IFFT and IDFT</B> for all sizes on the MAPs. The drivers for the MAP
 * in our reference soultion is <B style="color:blue;">very speific and catered to</B> these 3 functions. <B style="color:red;">It is not a
 * general purpose driver!!!</B> Since that the MAP code has been developed only for these 3 tasks and our current model does not allow
 * our customers to program on this MAP processor, we made the driver very application specific to save on MIPS <BR/><BR/>
 *
 * TABLE_START
 * ----------------------------------------------------------------------------------------------------------------------
 * |Item                         |IDFT                      |IFFT                        |FFT                           |
 * ----------------------------------------------------------------------------------------------------------------------
 * |Run on MAPs                  |0, 1, 2, 3, 4             |5, 6, 7, 8, 9               |5, 6, 7, 8, 9                 |
 * ----------------------------------------------------------------------------------------------------------------------
 * |FPDMA Mode                   |FEXP                      |FEXP                        |FEXP                          |
 * ----------------------------------------------------------------------------------------------------------------------
 * |Input precision              |16-bits                   |16-bits                     |16-bits                       |
 * ----------------------------------------------------------------------------------------------------------------------
 * |Output precision             |16-bits                   |16-bits                     |16-bits                       |
 * ----------------------------------------------------------------------------------------------------------------------
 * |Co-efficients precision      |16-bits                   |16-bits                     |16-bits                       |
 * ----------------------------------------------------------------------------------------------------------------------
 * |Intermediate stage precision |24-bits                   |24-bits                     |24-bits                       |
 * ----------------------------------------------------------------------------------------------------------------------
 * |Number of TCBs from TrEx/REX |1 per symbol              |1 for all symbols & antennas|1 for all symbols & antennas  |
 * ----------------------------------------------------------------------------------------------------------------------
 * |Number of Interrupts to ARM  |5-14. Depending on queuing|5. All descriptors are queued|5. All descriptors are queued|
 * |(from MAPs)                  |timing from TrEx/REX      |before dispatch              |before dispatch              |
 * ----------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 * <BR/><BR/>
 *
 *
 * The APIs that are part of the MAP drivers are listed in the table below and I/O params are explained in the Function Documentation section.
 * <BR/><BR/>
 *
 * TABLE_START
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |Function                    |General            |IFFT                       |FFT                       |IDFT                    |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |Reset                       |\ref MapDrvReset   |                           |                          |                        |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |Shutdown                    |\ref MapDrvShutdown |\ref MapIfftDrvShutdown   |\ref MapFftDrvShutdown    |\ref MapIdftDrvShutdown |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |Init                        |\ref MapDrvInit     |\ref MapIfftDrvInit       |\ref MapFftDrvInit        |\ref MapIdftDrvInit     |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |Set FP Mask                 |                   |\ref MapIfftDrvSetMapMask  |\ref MapFftDrvSetMapMask  |\ref MapIdftDrvSetMapMask |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |Set CallBack Function       |                   |\ref MapIfftDrvSetCallback |\ref MapFftDrvSetCallback |\ref MapIdftDrvSetCallback |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |Set FP ID to run next task  |                   |\ref MapIfftDrvSetFpNum    |\ref MapFftDrvSetFpNum    |\ref MapIdftDrvSetFpNum |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |Queue up or Run Task        |                   |\ref MapIfftDrvRun         |\ref MapFftDrvRun         |\ref MapIdftQueue       |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |Setup Number of Antennas    |                   |\ref MapIfftDrvSetNumAntennas |\ref MapFftDrvSetNumAntennas |                  |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |Add Input Buffers to Driver |                   |\ref MapIfftDrvSetInpBuf   |\ref MapFftDrvSetInpBuf   |                        |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |Add Output Buffers to Driver|                   |\ref MapIfftDrvSetOutBuf   |\ref MapFftDrvSetOutBuf   |                        |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |Number of symbols to process|                   |\ref MapIfftDrvSetNumSym   |\ref MapFftDrvSetNumSym   |                        |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 *
 *
 * <BR/><BR/>
 * </DIV>
 */

/**
 * \ingroup group_lte_hal
 * \defgroup group_lte_hal_fec FEC (Channel Coding) Driver Functions
 * @brief FEC (Channel Coding) Driver Functions
 *
 * <DIV style="text-align:justify;">
 * The Transcede&trade; 4000 product has 2 hardware engines called FEC (Forward Error Correction / Channel Coding) units. These were built to run the 4G
 * bit-level operations like Segmentation, Interleaving, Channel Coding, Rate Matching, etc. The 2 engines are called:
 *
 * - DL FEC: Downlink FEC
 * - UL FEC: Uplink FEC
 *
 * These units have hardware dispatchers which basically takes in a descriptor (C - structure) written into memory which defines the operation to be
 * performed by the unit. It is important to note that multiple descriptors could be linked together so as to increase performance of the FEC units and
 * reduce number of FEC <-> ARM interrupts. We call this process as chaining of the descriptors. It also helps in pipelining the various different
 * modules within each FEC so that the hardware is made use of efficiently. <BR/><BR/>
 *
 * We have a plethora of APIs for the FEC units as listed in table below and explained in the function Documentation section. <BR/><BR/>
 *
 * TABLE_START
 * ---------------------------------------------------------------------------------------------------------------------------
 * |Operation                  |General                       |DL FEC                        |UL FEC                         |
 * ---------------------------------------------------------------------------------------------------------------------------
 * |Init                       |\ref FecReset                 |\ref FecDlInit                |\ref FecUlInit                 |
 * |                           |\ref FecInit                  |                              |                               |
 * ---------------------------------------------------------------------------------------------------------------------------
 * |Allocate Descriptors       |                              |\ref FecDlAllocDesc           |\ref FecUlAllocDesc            |
 * |                           |                              |                              |\ref FecUlAllocNewDesc         |
 * ---------------------------------------------------------------------------------------------------------------------------
 * |Set Single I/O             |                              |\ref FecDlSetData             |\ref FecUlSetData              |
 * ---------------------------------------------------------------------------------------------------------------------------
 * |Run MDMA                   |                              |\ref FecDlRun                 |\ref FecUlRun                  |
 * |                           |                              |\ref FecDlDma                 |\ref FecUlDma                  |
 * ---------------------------------------------------------------------------------------------------------------------------
 * |Interrupt Handlers         |                              |\ref FecDlIrqHandler          |\ref FecUlSetLastDesc          |
 * ---------------------------------------------------------------------------------------------------------------------------
 * |Set IRQ Call Back          |                              |\ref FecDlSetLastDesc         |\ref FecUlIrqHandler           |
 * ---------------------------------------------------------------------------------------------------------------------------
 * |Process IRQs               |                              |\ref FecDlProcInt             |\ref FecUlProcInt              |
 * ---------------------------------------------------------------------------------------------------------------------------
 * |Shutdown                   |\ref FecShutdown              |\ref FecDlShutdown            |\ref FecUlShutdown             |
 * ---------------------------------------------------------------------------------------------------------------------------
 * |Get Hardware Context       |                              |\ref FecDlSetHwCtx            |\ref FecUlSetHwCtx             |
 * ---------------------------------------------------------------------------------------------------------------------------
 * |Master ARM CPU Identify    |                              |\ref FecDLGetMasterCpu        |\ref FecULGetMasterCpu         |
 * ---------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 *
 *
 * <BR/><BR/>
 * </DIV>
 */

/**
 * \ingroup group_lte_hal
 * \defgroup group_lte_hal_mdma MDMA (Memory to Memory DMA) Driver Functions
 * @brief MDMA (Memory to Memory DMA) Driver Functions
 *
 * <DIV style="text-align:justify;">
 * The Transcede&trade; 4000 product has 3 hardware engines called MDMA (Memory to Memory DMA) units. These were built solely to move data between
 * memories. In the process of moving, they can perform several low level 4G and 3G bit level processes like scrambling, descrambling, CRC calculation,
 * etc. The 3 engines are called:
 *
 * - SYS MDMA: General purpose MDMA
 * - SPU MDMA: General purpose MDMA
 * - RAD MDMA: Used solely for moving data between the Radio and Memory
 *
 * These units have hardware dispatchers which basically takes in a descriptor (C - structure) written into memory which defines the operation to be
 * performed by the unit. It is important to note that multiple descriptors could be linked together so as to increase performance of the MDMA units and
 * reduce number of MDMA <-> ARM interrupts. We call this process as chaining of the descriptors <BR/><BR/>
 *
 * We have a plethora of APIs for the MDMA units as listed in table below and explained in the function Documentation section. <BR/><BR/>
 *
 * TABLE_START
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Operation                  |General                       |SYS MDMA                      |SPU MDMA                       |RAD MDMA                       |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Init                       |\ref MdmaInit                 |\ref MdmaSysInit              |\ref MdmaSpuInit               |\ref MdmaRadInit               |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Allocate Descriptors       |                              |\ref MdmaSysAllocDesc         |\ref MdmaSpuAllocDesc          |\ref MdmaRadAllocDesc          |
 * |                           |                              |\ref MdmaSysGetSingleDesc     |\ref MdmaSpuGetSingleDesc      |\ref MdmaRadGetSingleDesc      |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Set Single I/O             |                              |\ref MdmaSetData           |\ref MdmaSpuSetData            |\ref MdmaSetData            |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Set Multiple I/O           |                              |\ref MdmaSetDataMultiple   |                               |\ref MdmaSetDataMultiple    |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Set Operation              |\ref MdmaSetCopy              |                              |                               |                               |
 * |                           |\ref MdmaSetCRC               |                              |                               |                               |
 * |                           |\ref MdmaSet4GSCR             |                              |                               |                               |
 * |                           |\ref MdmaSet4GDSCR            |                              |                               |                               |
 * |                           |\ref MdmaSetMultiple4GDSCR    |                              |                               |                               |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Run MDMA                   |                              |\ref MdmaSysRunEx             |\ref MdmaSpuRunEx              |\ref MdmaRadRunEx              |
 * |                           |                              |\ref MdmaSysStart             |\ref MdmaSpuStart              |\ref MdmaRadStart              |
 * |                           |                              |\ref MdmaSysStop              |\ref MdmaSpuStop               |\ref MdmaRadStop               |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Interrupt Handlers         |                              |\ref MdmaSysIrqHandler        |\ref MdmaSpuIrqHandler         |\ref MdmaRadIrqHandler         |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Set IRQ Call Back          |                              |\ref MdmaSysSetLastDesc       |\ref MdmaSpuSetLastDesc        |\ref MdmaRadSetLastDesc        |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Process IRQs               |                              |\ref MdmaSysProcInt           |\ref MdmaSpuProcInt            |\ref MdmaRadProcInt            |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Free Descriptors           |                              |\ref MdmaSysFreeDesc          |\ref MdmaSpuFreeDesc           |\ref MdmaRadFreeDesc           |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Shutdown                   |\ref MdmaShutdown             |\ref MdmaSysShutdown          |\ref MdmaSpuShutdown           |\ref MdmaRadShutdown           |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Get Hardware Context       |                              |\ref MdmaSysSetHwCtx          |\ref MdmaSpuSetHwCtx           |\ref MdmaRadSetHwCtx           |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Master ARM CPU Identify    |                              |\ref MdmaSysGetMasterCpu      |\ref MdmaSpuGetMasterCpu       |\ref MdmaRadGetMasterCpu       |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 *
 *
 * <BR/><BR/>
 * </DIV>
 */

//*****************************************************************************************************



/**
 * \defgroup group_lte_no_group Functions with No Group
 * @brief Functions with No Group Information (used for Fixing Doxygen comments in code)
 */

//*****************************************************************************************************
//*****************************************************************************************************
// Chapters
//---------

/**
 * \mainpage Table of Contents
 *
 * <DIV style="text-align:justify; font-size:13px;">
 *
 * \ref lte_introduction
 *
 * <BR/><BR/>
 *
 * \ref lte_reference_phy
 * <UL style="list-style-type: none;"><LI>
 * \ref lte_reference_phy_a
 * </LI></UL>
 *
 * <BR/>
 *
 * \ref phy_projects
 * <UL style="list-style-type: none;"><LI>
 * \ref phy_projects_a
 * </LI><LI>
 * \ref phy_projects_b
 * </LI><LI>
 * \ref phy_projects_c
 * </LI><LI>
 * \ref phy_projects_d
 * </LI></UL>
 *
 * <BR/>
 *
 * \ref phy_be_tests
 * <UL style="list-style-type: none;"><LI>
 * \ref phy_be_tests_a
 * </LI><LI>
 * \ref phy_be_tests_b
 * </LI><LI>
 * \ref phy_be_tests_c
 * </LI></UL>
 *
 * <BR/>
 *
 * \ref phy_documentation
 *
 * <BR/><BR/><BR/>
 *
 * <BR/><BR/>
 * </DIV>
 */

/**
 * \page lte_introduction 1. Introduction
 * <DIV style="text-align:justify;">
 *
 * <B style="font-size:15px;"><I>Product Status</I></B><BR/><BR/>
 * <I>Note: This product is still under development. For the current implementation status compared to
 * to the Product Definition below, see the Release Notes.</I>
 * <BR/><BR/><BR/>
 *
 * <B style="font-size:15px;"><I>Product Definition</I></B><BR/><BR/>
 * The Mindspeed LTE Reference PHY is a baseband PHY Reference Design for a 4G base station, using
 * the Transcede&trade; T4xxx SoC. This LTE Reference PHY consists of a library of c-callable functions
 * which run on, and are optimized for, the Transcede&trade; T4xxx Device and demonstrates the capabilities
 * of the Transcede&trade; T4xxx SoC for implementing the highest throughput modes and lowest latency modes of these standards.
 * Functionality of these library
 * functions is defined by the relevant sections in [Rel.8 of 3GPP TS 36.211, 212, 213 and 214]. Performance
 * of these functions meets the requirements defined by the base station conformance tests
 * in [Rel.8 of 3GPP TS 36.141]. This library of Mindspeed functions will be used by Mindspeed Partners
 * and Transcede&trade; T4xxx end-customers as a foundation for their own product development.
 *
 * </DIV>
 */

/**
 * \page lte_reference_phy 2. LTE Reference PHY
 * <DIV style="text-align:justify;">
 *
 * \section lte_reference_phy_a 2.1.  C-Code Style Guidelines
 * Coding style guidelines used throughout the LTE Reference PHY C-code are defined in the following
 * sections. These guidelines are designed to enhance readability and thereby improve the
 * understanding and maintainability of the code.
 *
 * <UL style="list-style-type: none;"><LI>
 * <B><I>2.1.1  Transcede&trade; General Naming Conventions</I></B>
 *
 * <UL style="list-style-type: none;"><LI>
 * <I>2.1.1.1  Variables</I>
 * - "p" is used for the first character for all pointers (e.g pPointer)
 * - "camelCasing" is used for local variables, with multi-word descriptive names
 * - "PascalCasing" is used for Global Variables
 * </LI>
 *
 * <LI>
 * <I>2.1.1.2  Functions</I>
 * - "PascalCasing" is used for function names
 * - "underscores" are used when separating an abbreviation from PascalCasing function name. (e.g LTE_Scrambler)
 * - "Abbreviations" 1-3 characters long use UPPERCASE, e.g. LTE_Scrambler
 * - "Abbreviations" >3 characters long use PascalCase, e.g. DtmfReceiver
 * </LI>
 *
 * <LI>
 * <I>2.1.1.3  Types & Constants</I>
 * - "PascalCasing" is used for struct, enum and class names
 * - "UPPERCASE" is used for type names and Constants. Underscores are used between words in multi-word descriptive names
 *    e.g.SPEECH_CHANNEL
 * - "P" prefix is used for pointer type names
 * </LI>
 *
 * </UL>
 *
 * <LI>
 * <B><I>2.1.2  Transcede&trade; Wireless PHY Naming Conventions</I></B>
 *
 * <UL style="list-style-type: none;"><LI>
 * <I>2.1.2.1  PHY Variables</I><BR>
 * Variables in the LTE PHY Standard documents are generally defined as single-letter mathematical symbols
 * with superscript and subscript parts. These single letter variables use upper case, lower case or greek letters.
 * A consistent method of mapping these symbolic names used in the standard document to variable names in the
 * LTE PHY Reference C-code is used, as follows:
 * -#   Preserve the (upper/lower) case of the variable name.
 *   -	In almost all cases, mathematical symbols defined in the PHY standards use a single letter name.
 * -#   If the variable is a greek letter, spell it out in English
 *   -	E.g.   ' beta
 * -#   All superscripts and subscripts are always converted to lower case, except for superscripts following
 *      a lowercase letter. Subscripts following a superscript are always preceded with the underscore character. For example:
 *   -	Name with {Uppercaseletter, superscript, subscript} is written Usuper_sub
 *   -	Name with {Lowercaseletter, superscript, subscript} is written lSuper_sub
 *   -	Name with {lowercaseletter, subscript} is written l_sub
 *   - Name with {Uppercaseletter, subscript} is written Lsub (i.e. no underscore before the subscript)
 * -#   These mathematical symbols defined in the PHY standard are generally only used in the equations defined
 * in the PHY standard in the lowest level PHY functions. There is no need to use these symbolic names at the
 * API level and throughout the higher levels of code, if a more descriptive name following the Transcede&trade; General Naming
 * Conventions makes the code more readable. For example:
 *   -	A function call:- Y = FuncA(numBitsInTransportBlock);
 *   - can be defined inside the function with prototype:- int = FuncA(int B), if B is the symbol used inside the
 *      function in the equation.
 * </LI>
 *
 * <LI>
 * <I>2.1.2.2  PHY data structures</I>
 * - Each data variable is defined to be in one of five top-level data structures:
 *   -	For BS: bs, bsTx, bsTxChan, bsRx, bsRxChan
 *   -	For MS: ms, msTx, msTxChan, msRx, msRxChan
 *
 * - So, all data variables "var" are located in one of these top level structures according to the following reasoning:
 *   -	For BS:
 *     -	bs.var - for variables common to both Tx and Rx
 *     -	bsTx.var - for variables common to all Tx channels for that BS
 *     -	bsTxChan.var - for variables specific to each Tx channel
 *     -	bsRx.var - for variables common to all Rx channels
 *     -	bsRxChan.var - for variables specific to each Rx channel
 *   -	For MS
 *     -	ms.var - for variables common to both Tx and Rx
 *     -	msTx.var - for variables common to all Tx channels for the BS connected to
 *     -	msTxChan.var - for variables specific to each Tx channel
 *     -	msRx.var - for variables common to all Rx channels for the BS connected to
 *     - 	msRxChan.var - for variables specific to each Rx channel
 * </LI></UL>
 *
 * <UL style="list-style-type: none;"><LI>
 * <I>2.1.2.3  Functions</I><BR>
 * Each function is named as:
 * - Prefix_MainPart( )
 * - MainPart of function name is the same as Agilent ADS/SystemVue equivalent function
 * - Prefix, component parts are attached in the following order:-
 *	- Phy	//only if this is interface to layer 2. i.e. visible to code outside the phy
 *	- Bs (or Ms) //if used for BS (or MS) only
 *	- Tx (or Rx) //if used for Tx (or Rx) only
 *	- UL //if used in both MsTx and BsRx (more readable than MsTxBsRx_)
 *	- DL //if used in both BsTx and MsRx (more readable than BsTxMsRx_)
 *
 * Prefix examples:
 * - PhyBsTx( )
 *  - top level BS Tx function. Starts with Phy since entry point from layer 2
 * - UL_Pilot( )
 *  - Pilot tone generation. Used in both MS Tx and in the BS Rx where the pilots are locally generated for channel estimation
 * - BsRx_ChEstimator( )
 *  - Channel Estimator used in the BS Rx
 * - Tx_Mapper( )
 *  - QAM mapper. Same for both directions, so same function is used in both BS Tx and MS Tx.
 * </LI>
 * </UL>
 * </LI>
 *
 * <LI>
 * <B><I>2.1.3  Function Parameter Tables</I></B><BR>
 * Within the Doxygen comments, a table giving more details for the function parameters is included.
 * This table, which has a row for each parameter, has columns to indicate:
 * -# <B><I>Name:</I></B> Name of the function parameter.
 * -# <B><I>Name in Standard:</I></B> If applicable, corresponding name of the function parameter as it appears in the 3GPP standard documents.
 * -# <B><I>Direction:</I></B> Indicates directional flow of parameter relative to the function.  The options are:
 *  - input 	function parameter is an input
 *  - output    function parameter is an output
 *  - input/output functioin parameter is both an input and output
 * -# <B><I>Length:</I></B> Length that corresponds to parameter. For arrays the length is defined as the number of array elements.
 *    For variables, values and structures the length would be 1.
 * -# <B><I>Type:</I></B> Indicates the type of function parameter. Some options are (where prefix "p" means "pointer to"):
 *  - Real, pReal, Real Array, pReal Array
 *  - Complex, pComplex, Complex Array, pComplex Array
 *  - Structure, pStructure, Structure Array, pStructure Array
 *  - pString, pFile
 * -# <B><I>Format:</I></B> Indicates the numeric precision and representation used for the parameter. The notation used is defined below.
 *  - <B>(S:I:F)</B>, where
 *   - S = number of sign bits.
 *   - I = number of bits used to represent integer part.
 *   - F = number of bits used to represent fractional part.
 *  - Some typical format types are shown below.
 *    - (0:8:0)		U8	(unsigned 8-bit integer, range = 0 to 255)
 *    - (1:7:0)		S8	(signed 8-bit integer, range = -128 to 127)
 *    - (0:0:8)		U8	(unsigned 8-bit fraction, range = 0/256 to 255/256)
 *    - (1:0:7)		S8	(signed 8-bit fraction, range = -128/128 to 127/128)
 * -# <B><I>Valid Range:</I></B> Specifies the valid numerical range or allowed values. Some options are:
 *  - full range		full numerical range supported by format is allowed
 *  - (A:B)			range from A (lower limit) to B (upper limit) exclusive
 *  - [A:B]			range from A (lower limit) to B (upper limit) inclusive
 *  - {A,B}			individual values A and B
 * -# <B><I>Units:</I></B> Enter type of units used by parameter (e.g. sec, Hz, MHz).  If not applicable, then enter "none" or leave blank.
 *
 * </LI></UL>
 *
 * <BR/><BR/>
 * </DIV>
 *
 */

/**
 * \page phy_projects 3.  LTE PHY Projects
 *
 * <DIV style="text-align:justify;">
 * This section explains all the relevant tools and projects used for compiling and running the PHY code. <BR/>
 *
 *
 * \section phy_projects_a 3.1  Modes of Operation
 * There are multiple modes in which the PHY code can be compiled and run under. <BR/>
 * - <B><I>C Only Mode</I></B>: Everything is simlated in C and run on PC <I>(also referred to as Host PC)</I>.
 * - <B><I>Ceva Simulation Mode</I></B>: All DSP fucntions that are defined to be run in the CEVAs on hardware are compiled to be run using CEVA simulator.
 * Everything else is compiled to run in C on PC <I>(also referred to as Host PC)</I>.
 * - <B><I>Hardware Mode</I></B>: All functions are compiled to run on Hardware (on ARM, CEVA, MAP, FEC, MDMA, etc). There are 2 projects in this mode
 *   - <B><I>LteProj</I></B>: This is used for code development wherein each task is run on resource and programmer can step into code using Hardware debuggers
 *   - <B><I>LteRT</I></B>: This is the Real Time project where the LTE PHY runs in real time (1ms TTI) and uses the 4GMX's threads
 *
 * <BR/><BR/>
 *
 * \section phy_projects_b 3.2  Compile Tools
 * Since there are multiple modes of running the Mindspeed LTE Referene Library, there are also multiple tools that are used to compile these sources and
 * associated projects. We in Mindspeed use Windows PC as the Host PC. Here is the list of tools that need to be installed to compile various projects
 *
 * <BR/><BR/>
 *
 *
 *
 * - <B style="font-size:13px;"><I style="text-decoration:underline;">3.2.1 Microsoft Visual C++ 2008 Express Edition&copy;</I></B>
 * <BR/><BR/><img src="Miscosoft_Visual_CPP_2008.png"/><BR/>
 * The Visual Studio&reg; 2008 Express is a set of free tools which offers users an exciting experience with the new integrated development environment,
 * a new editor built in Windows&reg; Presentation Foundation (WPF) and support for the new .NET Framework 4.<BR/><BR/>
 * <B>Download Link</B> <I>(as of this release)</I>: <A href="http://www.microsoft.com/express/Downloads/#2008-Visual-CPP" target="_blank" style="font-weight:bold;">http://www.microsoft.com/express/Downloads/#2008-Visual-CPP</A><BR/>
 * <B>Used for</B>: Mindspeed uses this for compiling and building all the projects and sources that are to be run on the Host PC <I>(C Only Mode and Ceva Simulation Mode)</I>.
 * <BR/><BR/>
 *
 * - <B style="font-size:13px;"><I style="text-decoration:underline;">3.2.2 Ceva SmartNCode&reg; SDT 9.2</I></B>
 * <BR/><BR/><img src="Ceva_SmartNCode.png"/><BR/>
 * CEVA-Toolbox<sup>TM</sup> constitutes a framework for software development for CEVA DSP Cores. CEVA-Toolbox includes a complete set of development,
 * debug and optimization tools for each of CEVA's DSP product families. All tools can be operated and configured via the integrated development environment (IDE)
 * using a Graphic User Interface, supporting the user through his entire embedded application development flow.<BR/><BR/>
 * Recently, the CEVA-Toolbox was extended with an innovative Application Optimizer toolchain that enables pure-C level application optimization for DSP applications.
 * With the addition of this tool chain, CEVA offers a complete, end-to-end C-level software development environment significantly improving application performance
 * and reducing time-to-market.<BR/><BR/>
 * <B>Download Link</B> <I>(as of this release)</I>: <A href="http://www.ceva-dsp.com/products/tools/software/index.php" target="_blank" style="font-weight:bold;">http://www.ceva-dsp.com/products/tools/software/index.php</A><BR/>
 * <B>Used for</B>: Mindspeed uses this for compiling and building all the projects and sources that are to be run on the Ceva <I>(Ceva Simulation Mode and Hardware Mode)</I>.
 * <BR/><BR/>
 *
 *
 * - <B style="font-size:13px;"><I style="text-decoration:underline;">3.2.3 ARM Workbench IDE v4.0</I></B>
 * <BR/><BR/><img src="ARM_Workbench.png"/><BR/>
 * The ARM&reg; Workbench IDE in RVDS <I>(RealView Development System&copy;)</I> is an IDE <I>(Integrated Development Environment)</I> based on the open-source Eclipse
 * 3.3 IDE. This combines Eclipse's outstanding source code development tools and plug-in framework with ARM's best-in-class compilation and debug technology.
 * Developers can use the ARM Workbench IDE project manager to create, build, debug, program Flash, profile, trace and manage C/C++ projects for all ARM processor-based
 * targets.<BR/><BR/>
 * The IDE contains many features that significantly increase productivity, including a C/C++ source browser configuration wizards, cheat sheets, and project stationery.
 * Project stationery enables a simple way to optimally configure the tools for specific ARM processors and development boards, and can be used for projects either
 * self-hosted or hosted by an OS like ARM Linux.<BR/><BR/>
 * <B>Download Link</B> <I>(as of this release)</I>: <A href="http://www.arm.com/products/tools/software-tools/rvds/arm-workbench-ide.php" target="_blank" style="font-weight:bold;">http://www.arm.com/products/tools/software-tools/rvds/arm-workbench-ide.php</A><BR/>
 * <B>Used for</B>: Mindspeed uses this for compiling and building all the projects and sources that are to be run on the ARM in Hardware <I>(Hardware Mode)</I>.
 * <BR/><BR/>
 *
 *
 * - <B style="font-size:13px;"><I style="text-decoration:underline;">3.2.4 CodeSourcery G++ Lite (GNU Toolchain for ARM Processors)</I></B>
 * <BR/><BR/><img src="CodeSourcery.png"/><BR/>
 * CodeSourcery, in partnership with ARM, Ltd., develops improvements to the GNU Toolchain for ARM processors and provides regular, validated releases
 * of the GNU Toolchain. Sourcery G++ Lite Edition supports ARM, Thumb, and Thumb-2 compilation for all architectures in active use, including Version 7
 * of the ARM Architecture. <BR/><BR/>
 * <B>Download Link</B> <I>(as of this release)</I>: <A href="http://www.codesourcery.com/sgpp/lite/arm" target="_blank" style="font-weight:bold;">http://www.codesourcery.com/sgpp/lite/arm</A><BR/>
 * <B>Used for</B>: Mindspeed uses this for compiling and building all the projects and sources that are to be run on the ARM (Linux) in Hardware <I>(Hardware Mode)</I>.
 * <BR/><BR/>
 *
 * <BR/><BR/>
 *
 *
 * \section phy_projects_c 3.3  Directory Structure
 * This section explains the directory structure of the Reference PHY delivered to customers
 *
 * - <B>T40xxSrc \ T40xxBin</B>: Source Customer Release \ Evaluation Customer Release main folder
 *   - <B>Common</B>: All sources that are common to all projects across the variuos folders
 *   - <B>LteTestMac</B>: The reference TestMac that is used to <B>test</B> various configurations of PHY in the device
 *   - <B>PHY</B>: All sources that belong to the actual Mindspeed Reference PHY layer
 *     - <B>Common</B>: All sources that are common to Mindspeed LTE / Wimax / 3G-WCDMA Reference PHYs
 *     - <B>LTE</B>: All sources that are common to Mindspeed LTE Reference PHYs
 *        - <B>Arm</B>: All sources and Projects used to enter the PHY layer within the device
 *           - <B>Prj\\LtePhyApp</B>: This folder contains sources used to run the PHY in a serial mode of execution. Mostly used while in development stage to easy testing and debugging
 *           - <B>Prj\\LtePhyRt</B>: This folder contains sources used to run the PHY in a real-time mode using the OS's threads and TrEx's / REX's interrupts.
 *        - <B>C-Code</B>: All Sources, Projects and Test Scripts for the LTE Reference PHY
 *           - <B>Lib</B>: Pre-built library's and DLL's which are provided as part of release
 *           - <B>Prj</B>: All projects that are built and run from Host PC for testing the LTE Reference PHY
 *           - <B>Unit_Test</B>: All Test Scripts used to test the LTE Reference PHY
 *        - <B>Ceva</B>: All Sources (assembly) and Projects that are compiled and run on the Ceva DSP
 *           - <B>Prj</B>: All projects that are built for the Ceva
 *             - <B>App</B>: Empty Sample Ceva Project (with Mindspeed Entry Points) which can be used by customers to try some example code to evaluate the Ceva
 *             - <B>CevaBsLib</B>: Project that is used to build all the lower level PHY functions within the Ceva. This creates a .lib
 *             - <B>LteApp</B>: Project that contains the top level "entry" functions into the Ceva from Mindspeed's TrEx / REX. It includes the CevaBsLib.lib while linking
 *   - <B>Redist</B>: Contains all the re-distributable files that can be used by customers to include Microsoft libraries neeeded for running on Host PC
 *   - <B>T4KDev</B>: This folder contains the Lower Sub System functions (4GMX Operating System, TrEx / REX, Device Drivers, Diagnostic Codes, etc.)
 *     - <B>4GMX</B>: The Lower Sub System Operating System folder
 *     - <B>BIN</B>: Precompiled binaries which could be used to link with the PHY
 *     - <B>LTEProj</B>: This project is used to run the PHY in a serial mode of execution. Mostly used while in development stage to easy testing and debugging
 *     - <B>LTERT</B>: This project is used to run the PHY in a real-time mode using the OS's threads and TrEx's / REX's interrupts.
 *     - <B>STUBProj</B>:
 *     - <B>SYSCORE</B>: This fodler contains sources to TrEx / REX and all Device Drivers
 *       - <B>DRIVERS</B>: Contains all the drivers to modules used by Reference PHY in device
 *         - <B>CPRI</B>: CPRI Drivers
 *         - <B>FEC</B>: FEC and MDMA Drivers
 *         - <B>MAP</B>: MAP Drivers
 *       - <B>SERVICES</B>: Contains the TrEx / REX and all system Bootup Diagnostics
 *     - <B>T4KApp</B>: This folder contains sources to the stand-along system diagnostic tests
 *   - <B>TCBexample</B>: This folder contains a simple example to usage of the TrEx / REX APIs with functions scheduled to run in ARM and Ceva
 *   - <B>Tools</B>: This folder contains all tools developed and delivered by Mindspeed that are useful for Debugging and Evaluating the Device.
 *   - <B>TurnerProject</B>: This folder contains the project used to build the Mindspeed TestMac
 *
 *
 * <BR/><BR/>
 *
 * \section phy_projects_d 3.4  Projects
 * This section is aimed at explaining the purpose of the various projects that are delivered as part of the Reference PHY which customers could re-build.
 * <BR/><BR/>
 *
 * The diagram below shows the various stages that are simulated by the several projects in the Refernce PHY delivered. These numbers from the diagram will
 * be referenced in the sections to follow <B><I>(Simulating Column in tables below)</I></B>:<BR/><BR/>
 *
 * <img src="LTE_Projects.jpg"/>
 * <BR/><BR/>
 *
 * - <B style="font-size:13px; color:blue;"><I style="text-decoration:underline;">3.4.1 Host Projects</I></B>
 * All our host projects are built using <B><I>Microsoft Visual Studio 2008 Express Edition &copy;</I></B>.
 * There are .sln files <I>(which are workspaces)</I> included in the customer release folders which are opened by this version of the tool. <BR/><BR/>
 * <B>LTE Referenece PHY Projects</B>: These projects are used to compile sources for testing the Reference PHY with various different test options<BR/>
 * <B>Location of Projects</B>: PHY\\LTE\\C-Code\\Prj<BR/>
 * <B>Uses Sources from</B>: Common, PHY\\LTE\\C-Code, PHY\\LTE\\C-Code\\Prj\\*, PHY\\Common\\C-Code, T4kDev\\SYSCORE\\SERVICES<BR/>
 * <BR/><BR/>
 *
 * TABLE_START
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Project Name           |Simulating |Mode(s) used in |Depends on  |Description                                                                                     |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |4GCPlugin_LTE_PHY      |1, 2       |C-Only          |BsLib,      |DLL to run PHY in c-only mode.                                                                  |
 * |                       |           |                |BsLibMspd   |- Both Debug and Release can be compiled.                                                       |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |4GLowCore_LTE_PHY_pi   |3          |Hardware        |BsLib       |Interface DLL for host application to communicate with PHY on the EVM.                          |
 * |                       |           |                |FPGALib,    |- Only Debug and Release can be compiled.                                                       |
 * |                       |           |                |MacLib      |- EVMDebug and EVMRelease are not supported in CustomerRelease. These are used for HBI mode.    |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |4GPlugin_LTE_PHY       |2          |Ceva Sim        |BsLib,MacLib|DLL to run PHY with Ceva Simulator Mode.                                                        |
 * |                       |           |                |FPGALib,    |- Only CustomerDebug and CustomerRelease can be compiled.                                       |
 * |                       |           |                |BsLibMspd   |- Debug and Release are not supported in CustomerRelease.                                       |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |BsLib                  |2          |C-only,         |            |Library containing DSP functions for PHY                                                        |
 * |                       |           |Ceva-Sim,       |            |                                                                                                |
 * |                       |           |Hardware        |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |BsLibMspd              |2          |C-only,         |            |Library containing Advanced Proprietary MindSpeed DSP functions for PHY                         |
 * |                       |           |Ceva-Sim,       |            |                                                                                                |
 * |                       |           |Hardware        |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |FPGAlib                |2, 3       |Ceva-Sim,       |            |Library containing functions to commmunicate with Ceva-Simulator, SCTP connections and EVM      |
 * |                       |           |Hardware        |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Lte_BER_Test           |1          |C-only,         |            |Host test application to run Base Station Uplink Rx Bit Error Test for C-only,                  |
 * |                       |           |Ceva-Sim,       |            |Ceva-Sim and EVM modes.                                                                         |
 * |                       |           |Hardware        |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Lte_TestDL             |1          |C-only,         |            |Host test application to run Dowlink Direction bit exact testing for C-only, Ceva-Sim and       |
 * |                       |           |Ceva-Sim,       |            |EVM modes.                                                                                      |
 * |                       |           |Hardware        |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Lte_TestFD             |1          |                |            |Host test application to run LTE Full Duplex LTE bit exact test for C-only, Ceva-Sim and        |
 * |                       |           |Ceva-Sim,       |            |EVM modes.                                                                                      |
 * |                       |           |Hardware        |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Lte_TestUL             |1          |                |            |Host test application to run Uplink Direction bit exact testing for C-only, Ceva-Sim and        |
 * |                       |           |Ceva-Sim,       |            |EVM modes.                                                                                      |
 * |                       |           |Hardware        |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Maclib                 |1          |C-only,         |            |Library containing MAC functions                                                                |
 * |                       |           |Ceva-Sim        |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |RunBERTests            |2          |C-only          |            |Host Test application to run Bit Error Rate testing on Channel Coding section only.             |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |T4kLib                 |3          |Hardware        |            |Library containing functions to communicate with HBI EVM boards.                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |LtePhySrc\LtePhyEval   |           |                |            |Solutions for Visual studio 2008 containing all projects for compilation.                       |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 *
 * <BR/><BR/>
 *
 * <B>Tools Projects</B>: Thes projects are used to compile sources for tools used by the Mindspeed Reference PHY
 * <B>Location of Projects</B>: Tools\\*<BR/>
 * <B>Uses Sources from</B>: Tools\\*, PHY\\*, Common\\*, LteTestMac\\*, TurnerProject\\*<BR/>
 * <BR/><BR/>
 *
 * TABLE_START
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Project Name           |Simulating |Mode(s) used in |Depends on  |Description                                                                                     |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |4GMindDAL              |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |4GMindDspSim           |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |4GMindSCTP             |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |LTE_BE_Throughput      |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |LTE_BitError           |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |RouterTest             |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |BSCT                   |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |LTE_BitExact           |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |BSCT                   |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |tmonitor               |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |turnerio               |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |userio                 |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |trace                  |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |cevaldr                |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |sctp_service           |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |sctp_msg               |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |sctp_lib               |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |libvsa_demo            |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |libvsa                 |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 *
 * <BR/><BR/>
 *
 * - <B style="font-size:13px; color:blue;"><I style="text-decoration:underline;">3.4.2 Ceva Projects</I></B>
 * All our Ceva projects are built using <B><I>Ceva SmartNCode&copy;</I></B>.
 * There are .scs files <I>(which are workspaces)</I> included in the customer release folders which are opened by this version of the tool. <BR/><BR/>
 * <B>LTE Referenece PHY Projects</B>: These projects are used to compile Ceva sources for the Reference PHY<BR/>
 * <B>Location of Projects</B>: PHY\\LTE\\Ceva\\Prj<BR/>
 * <B>Uses Sources from</B>: Common, PHY\\LTE\\Ceva\\, PHY\\LTE\\Ceva\\Prj\\*, PHY\\LTE\\C-Code, PHY\\Common\\C-Code, T4kDev\\SYSCORE\\SERVICES<BR/>
 * <BR/><BR/>
 *
 * TABLE_START
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Project Name           |Simulating |Mode(s) used in |Depends on  |Description                                                                                     |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |App                    |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |CevaBsLibMspd          |2          |Ceva-Sim,       |            |Library containing Advanced Proprietary MindSpeed DSP functions for PHY                         |
 * |                       |           |Hardware        |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |CevaBsLib              |2          |Ceva-Sim,       |            |Library containing DSP functions compiled for Ceva.                                             |
 * |                       |           |Hardware        |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |LteApp                 |2          |Ceva-Sim,       |CevaBsLib   |Code containing entry point into all DSP functions for Ceva                                     |
 * |                       |           |Hardware        |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 *
 * <BR/><BR/>
 *
 * - <B style="font-size:13px; color:blue;"><I style="text-decoration:underline;">3.4.3 ARM Projects</I></B>
 * All our ARM projects are built using <B><I>ARM Workbench IDE&reg;</I></B>. Users also have the option to build it using standard Eclipse (using ARM compilers)
 * if they are comfortable with this GUI interface. There are .project and .cproject files <I>(which are workspaces)</I> included in the customer release folders
 * which are opened by this version of the tool. <BR/><BR/>
 * <B>LTE Referenece PHY Projects</B>: These projects are used to compile ARM PHY sources for the Reference PHY<BR/>
 * <B>Location of Projects</B>: T4kDev<BR/>
 * <B>Uses Sources from</B>: Common, PHY\\LTE\\*, T4kDev\\*<BR/>
 * <BR/><BR/>
 *
 * TABLE_START
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Project Name           |Simulating |Mode(s) used in |Depends on  |Description                                                                                     |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |                       |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |                       |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |                       |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 *
 * <BR/><BR/>
 *
 *
 * <B>LTE TestMac Projects</B>: These projects are used to compile ARM TestMac sources to test the Reference PHY on device<BR/>
 * <B>Location of Projects</B>: TurnerProject<BR/>
 * <B>Uses Sources from</B>: LteTestMac, TurnerProject<BR/>
 * <BR/><BR/>
 *
 * TABLE_START
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Project Name           |Simulating |Mode(s) used in |Depends on  |Description                                                                                     |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |                       |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |                       |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |                       |           |                |            |                                                                                                |
 * --------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 *
 * <BR/><BR/>
 *
 * <BR/><BR/>
 *
 * </DIV>
 */

/**
 * \page phy_be_tests 4.  Transcede&trade; LTE PHY Test Suite
 *
 * <DIV style="text-align:justify;">
 * This section defines the configuration parameters for all the Tests MSPD provides as part of the
 * release package. <BR/><BR/>
 *
 * \section phy_be_tests_a 4.1  DL Test Suite
 * <BR/>
 * These are DL stand alone tests that are run using the Lte_TestDL application in all 4 modes (C-Only, Ceva Sim and Hardware [LteProj and LteRT]).
 * There are batch files that look like <B><I>c_dl_test*.bat / sim_dl_test*.bat / hw_dl_test*.bat / rt_dl_test*.bat</I></B>
 * (where '*' is the test number defined in table below) which have been configured to the specifications listed in table
 * below and can be used to run the tests.
 * <BR/><BR/><BR/>
 *
 * TABLE_START
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |Test   |Bandwidth |Modulation |Num Tx            |Num PDSCH   |Num RBs |Num PDCCH |Num Control|Num PBCH  |Num PHICH |RT Mode   |
 * |Number |          |Type       |Antennas          |Channels    |        |Channels  |Symbols    |Channels  |Channels  |TTI       |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL3    |20 Mhz    |16 QAM     |Single            |1           |        |1         |3          |1         |          |          |
 * |\anchor DL3 |     |           |                  |(203 bytes) |12      |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL4    |10 Mhz    |64 QAM     |Single            |1           |        |1         |3          |1         |          |          |
 * |\anchor DL4 |     |           |                  |(1419 bytes)|50      |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL5    |10 Mhz    |64 QAM     |Spatial Multiplex |1           |        |1         |3          |1         |          |          |
 * |\anchor DL5 |     |           |2 Ant MIMO        |(1419 bytes)|29      |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL6    |10 Mhz    |64 QAM     |Single            |6           |        |          |           |1         |          |          |
 * |\anchor DL6 |     |           |                  |(200 bytes) |6       |          |           |          |          |          |
 * |            |     |           |                  |(220 bytes) |6       |          |           |          |          |          |
 * |            |     |           |                  |(230 bytes) |7       |          |           |          |          |          |
 * |            |     |           |                  |(240 bytes) |10      |          |           |          |          |          |
 * |            |     |           |                  |(250 bytes) |7       |          |           |          |          |          |
 * |            |     |           |                  |(260 bytes) |7       |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL7    |10 Mhz    |QPSK       |Single            |            |        |1         |3          |1         |          |          |
 * |       |          |           |                  |            |        |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL8    |10 Mhz    |QPSK       |Single            |            |        |          |           |1         |          |          |
 * |       |          |           |                  |            |        |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL9    |10 Mhz    |QPSK       |Single            |            |        |1         |3          |          |16        |          |
 * |       |          |           |                  |            |        |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL11   |10 Mhz    |64 QAM     |Single            |1           |        |1         |3          |1         |16        |          |
 * |\anchor DL16 |    |           |                  |(1419 bytes)|50      |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL12   |10 Mhz    |QPSK       |Single            |            |        |1         |1          |          |4         |          |
 * |       |          |           |                  |            |        |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL13   |10 Mhz    |QPSK       |Single            |            |        |1         |1          |          |4         |          |
 * |       |          |           |                  |            |        |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL14   |20 Mhz    |QPSK       |Single            |            |        |1         |1          |          |6         |          |
 * |       |          |           |                  |            |        |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL15   |20 Mhz    |64 QAM     |Single            |1           |        |6         |1          |1         |6         |          |
 * |\anchor DL15 |    |           |                  |(8910 bytes)|100     |(4 bytes) |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL16   |10 Mhz    |64 QAM     |Single            |1           |        |5         |1          |1         |4         |          |
 * |\anchor DL16 |    |           |                  |(4455 bytes)|50      |(4 bytes) |           |(3 bytes) |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL18   |20 Mhz    |QPSK       |Single            |            |        |1         |3          |          |29        |          |
 * |       |          |           |                  |            |        |(4 bytes) |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL20   |10 Mhz    |QPSK       |Single            |            |        |          |1          |1         |          |          |
 * |(10 sf)|          |           |                  |            |        |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL22   |10 Mhz    |64 QAM     |Single            |1           |        |1         |1          |1         |4         |          |
 * |\anchor DL22 |    |           |                  |(4455 bytes)|50      |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL23   |10 Mhz    |64 QAM     |Spatial Multiplex |1           |        |1         |1          |1         |          |          |
 * |\anchor DL23 |    |           |2 Ant MIMO        |(4711 x 2 bytes)|50  |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL24   |10 Mhz    |64 QAM     |Tx Diversity      |1           |        |1         |1          |1         |          |          |
 * |\anchor DL24 |    |           |2 Ant MIMO        |(4711 bytes)|50      |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL30   |10 Mhz    |64 QAM     |Single            |1           |        |1         |1          |1         |4         |          |
 * |\anchor DL30 |    |           |                  |(549 bytes) |6       |(4 bytes) |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL31   |10 Mhz    |64 QAM     |Single            |1           |        |1         |1          |1         |4         |          |
 * |\anchor DL31 |    |           |                  |(4455 bytes)|50      |(4 bytes) |           |(18 bytes)|          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL34   |20 Mhz    |64 QAM     |Spatial Multiplex |1           |        |1         |1          |1         |          |          |
 * |\anchor DL34 |    |           |2 Ant MIMO        |(9361 x 2 bytes)|100     |(4 bytes) |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL35   |20 Mhz    |64 QAM     |Spatial Multiplex |1           |        |1         |1          |1         |          |          |
 * |\anchor DL35 |    |           |2 Ant MIMO        |(5461 x 2 bytes)|60      |(4 bytes) |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL36   |20 Mhz    |64 QAM     |Spatial Multiplex |1           |        |1         |1          |1         |          |          |
 * |\anchor DL36 |    |           |2 Ant MIMO        |(9361 x 2 bytes)|100    |(4 bytes) |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL37   |20 Mhz    |64 QAM     |Spatial Multiplex |1           |        |1         |1          |1         |          |          |
 * |\anchor DL37 |    |           |2 Ant MIMO        |(4587 x 2 bytes)|50      |(4 bytes) |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL38   |20 Mhz    |64 QAM     |Spatial Multiplex |1           |        |1         |1          |1         |          |          |
 * |\anchor DL38 |    |           |2 Ant MIMO        |(5461 x 2 bytes)|84      |(4 bytes) |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL39   |20 Mhz    |64 QAM     |Spatial Multiplex |1           |        |1         |1          |1         |          |          |
 * |\anchor DL39 |    |           |2 Ant MIMO        |(6365 x 2 bytes)|100      |(4 bytes) |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL45   |10 Mhz    |           |Single            |8           |        |8         |1          |1         |6         |          |
 * |       |          |QPSK       |                  |(49  bytes) |3       |(4 bytes) |           |(18 bytes)|          |          |
 * |       |          |16 QAM     |                  |(113 bytes) |4       |          |           |          |          |          |
 * |       |          |64 QAM     |                  |(357 bytes) |5       |          |           |          |          |          |
 * |       |          |QPSK       |                  |(22  bytes) |3       |          |           |          |          |          |
 * |       |          |16 QAM     |                  |(153 bytes) |4       |          |           |          |          |          |
 * |       |          |64 QAM     |                  |(469 bytes) |5       |          |           |          |          |          |
 * |       |          |QPSK       |                  |(18  bytes) |3       |          |           |          |          |          |
 * |       |          |16 QAM     |                  |(85  bytes) |4       |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL110  |1.4 Mhz   |QPSK       |Single            |1           |        |2         |2          |1         |4         |          |
 * |E-TM1.1|          |           |                  |(117 bytes) |6       |(4 bytes) |           |(18 bytes)|          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL111  |3 Mhz     |QPSK       |Single            |1           |        |2         |1          |1         |4         |          |
 * |E-TM1.1|          |           |                  |(293 bytes) |15      |(4 bytes) |           |(18 bytes)|          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL112  |5 Mhz     |QPSK       |Single            |1           |        |2         |1          |1         |4         |          |
 * |E-TM1.1|          |           |                  |(501 bytes) |25      |(4 bytes) |           |(18 bytes)|          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL113  |10 Mhz    |QPSK       |Single            |1           |        |5         |1          |1         |4         |          |
 * |E-TM1.1|          |           |                  |(999 bytes) |50      |(4 bytes) |           |(18 bytes)|          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL115  |20 Mhz    |QPSK       |Single            |1           |        |6         |1          |1         |6         |          |
 * |E-TM1.1|          |           |                  |(1980 bytes)|100     |(4 bytes) |           |(18 bytes)|          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL11511|20 Mhz    |QPSK       |Single            |1           |        |6         |1          |1         |6         |          |
 * |E-TM1.1|          |           |                  |(1980 bytes)|100     |(4 bytes) |           |(18 bytes)|          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL123  |10 Mhz    |QPSK       |Single            |2           |        |5         |1          |1         |4         |          |
 * |E-TM1.2|          |           |                  |(389 bytes) |20      |(4 bytes) |           |(18 bytes)|          |          |
 * |       |          |           |                  |(579 bytes) |30      |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL205  |20 Mhz    |QPSK       |Single            |1           |        |6         |1          |1         |6         |          |
 * |E-TM2  |          |           |                  |(89 bytes)  |1       |(4 bytes) |           |(18 bytes)|          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL313  |10 Mhz    |64 QAM     |Single            |1           |        |5         |1          |1         |4         |          |
 * |E-TM3.1|          |           |                  |(4587 bytes)|50      |(4 bytes) |           |(18 bytes)|          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL323  |10 Mhz    |16 QAM     |Single            |2           |        |5         |1          |1         |4         |          |
 * |E-TM3.2|          |           |                  |(1143 bytes)|30      |(4 bytes) |           |(18 bytes)|          |          |
 * |       |          |           |                  |(389 bytes) |20      |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * |DL333  |10 Mhz    |QPSK       |Single            |2           |        |5         |1          |1         |4         |          |
 * |E-TM3.3|          |           |                  |(501 bytes) |25      |(4 bytes) |           |(18 bytes)|          |          |
 * |       |          |           |                  |(967 bytes) |25      |          |           |          |          |          |
 * ---------------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 *
 * <BR/><BR/>
 *
 * \section phy_be_tests_b 4.2  UL Test Suite
 * <BR/>
 * These are UL stand alone tests that are run using the Lte_TestUL application in all 4 modes (C-Only, Ceva Sim and Hardware [LteProj and LteRT]).
 * There are batch files that look like <B><I>c_ul_test*.bat / sim_ul_test*.bat / hw_ul_test*.bat / rt_ul_test*.bat</I></B>
 * (where '*' is the test number defined in table below) which have been configured to the specifications listed in table
 * below and can be used to run the tests.
 * <BR/><BR/><BR/>
 *
 * TABLE_START
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |Test   |Bandwidth |Modulation |Num Rx            |Num PUSCH   |Num RBs |Num PUCCH |PUCCH   |Num SRS    |Num PRACH  |Channel  |RT Mode   |
 * |Number |          |Type       |Antennas          |Channels    |PUSCH   |Channels  |RBAssign|Signals    |Channels   |Model    |TTI       |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL1    |5 Mhz     |16 QAM     |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |(MRC)             |(97 bytes)  |4       |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL2    |10 Mhz    |16 QAM     |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |\anchor UL2 |     |           |(MRC)             |(1692 bytes)|32      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL4    |10 Mhz    |QPSK       |2 Rx Ant          |            |        |1         |        |           |           |         |          |
 * |       |          |           |(MRC)             |            |        |Format 2  |0       |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL5    |10 Mhz    |QPSK       |2 Rx Ant          |            |        |1         |        |           |           |         |          |
 * |       |          |           |(MRC)             |            |        |Format 2a |0       |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL6    |10 Mhz    |QPSK       |2 Rx Ant          |            |        |1         |        |           |           |         |          |
 * |       |          |           |(MRC)             |            |        |Format 2b |0       |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL7    |5 Mhz     |           |2 Rx Ant          |            |        |          |        |1          |           |         |          |
 * |       |          |           |                  |            |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL8    |10 Mhz    |           |2 Rx Ant          |            |        |          |        |8          |           |         |          |
 * |       |          |           |                  |            |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL9    |10 Mhz    |16 QAM     |2 Rx Ant          |1           |        |          |        |           |1          |         |          |
 * |       |          |           |(MRC)             |(751 bytes) |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL10   |10 Mhz    |64 QAM     |2 Rx Ant          |            |        |1         |        |           |           |         |          |
 * |       |          |           |(MRC)             |            |        |Format 1  |49      |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL11   |10 Mhz    |64 QAM     |2 Rx Ant          |            |        |1         |        |           |           |         |          |
 * |       |          |           |(MRC)             |            |        |Format 1a |49      |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL12   |10 Mhz    |64 QAM     |2 Rx Ant          |            |        |1         |        |           |           |         |          |
 * |       |          |           |(MRC)             |            |        |Format 1b |49      |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL13   |10 Mhz    |QPSK       |1 Rx Ant          |1           |        |          |        |           |           |Ext-TU   |          |
 * |       |          |           |                  |(13 bytes)  |1       |          |        |           |           |70 Hz    |          |
 * |       |          |           |                  |            |        |          |        |           |           |-2.5dB   |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL14   |10 Mhz    |QPSK       |2 Rx Ant          |1           |        |          |        |           |           |Ext-TU   |          |
 * |       |          |           |(MRC)             |(13 bytes)  |1       |          |        |           |           |70 Hz    |          |
 * |       |          |           |                  |            |        |          |        |           |           |-2.5dB   |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL16   |10 Mhz    |64 QAM     |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |(MRC)             |(4587 bytes)|50      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL17   |20 Mhz    |16 QAM     |1 Rx Ant          |1           |        |          |        |           |           |Ext-VA   |          |
 * |       |          |           |                  |(5477 bytes)|100     |          |        |           |           |70 Hz    |          |
 * |       |          |           |                  |            |        |          |        |           |           |10.6dB   |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL18   |20 Mhz    |16 QAM     |2 Rx Ant          |1           |        |          |        |           |           |Ext-VA   |          |
 * |       |          |           |(MRC)             |(5477 bytes)|100     |          |        |           |           |70 Hz    |          |
 * |       |          |           |                  |            |        |          |        |           |           |7.5dB    |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL20   |10 Mhz    |16 QAM     |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |(5 sf) |          |           |                  |(2292 bytes)|50      |          |        |           |           |         |          |
 * |       |          |           |                  |11 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL21   |10 Mhz    |64 QAM     |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |(5 sf) |          |           |                  |(2292 bytes)|50      |          |        |           |           |         |          |
 * |       |          |           |                  |12 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL22   |10 Mhz    |64 QAM     |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |\anchor UL22 |    |           |                  |(4597 bytes)|50      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL23   |10 Mhz    |           |2 Rx Ant          |2           |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |                  |(1527 bytes)|20      |          |        |           |           |         |          |
 * |       |          |16 QAM     |                  |(1335 bytes)|25      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL24   |10 Mhz    |           |2 Rx Ant          |2           |        |2         |        |           |           |         |          |
 * |       |          |64 QAM     |                  |(1527 bytes)|20      |Format 2  |0       |           |           |         |          |
 * |       |          |16 QAM     |                  |(1335 bytes)|25      |Format 1a |49      |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL25   |10 Mhz    |           |2 Rx Ant          |            |        |4(same RB)|        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1a |49      |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1a |49      |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1a |49      |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1a |49      |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL26   |10 Mhz    |           |2 Rx Ant          |            |        |1(SR/1a)  |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1  |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1a |49,1a RB|           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL27   |10 Mhz    |           |2 Rx Ant          |            |        |1(SR/1a)  |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1  |SR RB   |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1a |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL30   |10 Mhz    |64 QAM     |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(549 bytes) |6       |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL31   |10 Mhz    |64 QAM     |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |\anchor UL31 |    |           |                  |(4587 bytes)|50      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL32   |10 Mhz    |16 QAM     |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |\anchor UL32 |    |           |                  |(2385 bytes)|45      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL33   |20 Mhz    |64 QAM     |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |\anchor UL33 |    |           |                  |(4587 bytes)|100     |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL34   |20 Mhz    |64 QAM     |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |(MRC)             |(4587 bytes)|100     |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL35   |20 Mhz    |64 QAM     |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |\anchor UL35 |    |           |(MRC)             |(4587 bytes)|60      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL36   |20 Mhz    |64 QAM     |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |\anchor UL36 |    |           |(MRC)             |(9422 bytes)|100     |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL37   |20 Mhz    |64 QAM     |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |\anchor UL37 |    |           |(MRC)             |(4587 bytes)|50      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL38   |20 Mhz    |64 QAM     |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |\anchor UL38 |    |           |(MRC)             |(3302 bytes)|36      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL39   |20 Mhz    |64 QAM     |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |\anchor UL39 |    |           |(MRC)             |(6594 bytes)|72      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL40   |10 Mhz    |           |1 Rx Ant          |5           |        |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(533 bytes) |10      |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(533 bytes) |10      |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(533 bytes) |10      |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(533 bytes) |10      |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(533 bytes) |10      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL41   |10 Mhz    |           |1 Rx Ant          |10          |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |All 10 are  |All 10  |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(469 bytes) |are 5RBs|          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL42   |20 Mhz    |           |1 Rx Ant          |30          |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |11 (89bytes)|1       |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |8 (185bytes)|2       |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |3 (277bytes)|3       |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |3 (373bytes)|4       |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |1 (469bytes)|5       |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |2 (849bytes)|6       |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |1 (935bytes)|10      |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |1(2292bytes)|25      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL43   |20 Mhz    |64 QAM     |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |\anchor UL43 |    |           |(MRC)             |(4587 bytes)|50      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL44   |20 Mhz    |           |2 Rx Ant          |2           |        |2         |        |           |           |         |          |
 * |\anchor UL44 | |  |64 QAM     |                  |(1527 bytes)|20      |Format 2  |0       |           |           |         |          |
 * |       |          |16 QAM     |                  |(1335 bytes)|25      |Format 1a |99      |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL45   |10 Mhz    |           |2 Rx Ant          |4           |        |4         |        |           |1          |         |          |
 * |       |          |64 QAM     |                  |23(373bytes)|6       |          |        |           |           |         |          |
 * |       |          |16 QAM     |                  |12 (97bytes)|4       |          |        |           |           |         |          |
 * |       |          |QPSK       |                  |4  (26bytes)|3       |          |        |           |           |         |          |
 * |       |          |16 QAM     |                  |15(533bytes)|15      |          |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 2  |0       |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1a |49      |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1a |1       |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1a |48      |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL48   |10 Mhz    |QPSK       |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(173 bytes) |50      |          |        |           |           |         |          |
 * |       |          |           |                  |55 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL49   |10 Mhz    |QPSK       |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(225 bytes) |50      |          |        |           |           |         |          |
 * |       |          |           |                  |23 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL50   |10 Mhz    |QPSK       |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(277 bytes) |50      |          |        |           |           |         |          |
 * |       |          |           |                  |12 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL51   |10 Mhz    |QPSK       |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(357 bytes) |50      |          |        |           |           |         |          |
 * |       |          |           |                  |11 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL52   |10 Mhz    |QPSK       |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(453 bytes) |50      |          |        |           |           |         |          |
 * |       |          |           |                  |6 bits CQI  |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL53   |10 Mhz    |QPSK       |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(549 bytes) |50      |          |        |           |           |         |          |
 * |       |          |           |                  |3 bits CQI  |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL54   |10 Mhz    |64 QAM     |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(2673 bytes)|50      |          |        |           |           |         |          |
 * |       |          |           |                  |11 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL55   |10 Mhz    |64 QAM     |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(2865 bytes)|50      |          |        |           |           |         |          |
 * |       |          |           |                  |12 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL56   |10 Mhz    |64 QAM     |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(3182 bytes)|50      |          |        |           |           |         |          |
 * |       |          |           |                  |54 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL57   |10 Mhz    |64 QAM     |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(3422 bytes)|50      |          |        |           |           |         |          |
 * |       |          |           |                  |22 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL58   |10 Mhz    |64 QAM     |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(3542 bytes)|50      |          |        |           |           |         |          |
 * |       |          |           |                  |10 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL59   |10 Mhz    |64 QAM     |1 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |       |          |           |                  |(3822 bytes)|50      |          |        |           |           |         |          |
 * |       |          |           |                  |5 bits CQI  |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL60   |10 Mhz    |           |1 Rx Ant          |4           |        |          |        |           |           |         |          |
 * |       |          |QPSK       |(MRC)             |(11 bytes)  |4       |          |        |           |           |         |          |
 * |       |          |           |                  |11 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(173 bytes) |8       |          |        |           |           |         |          |
 * |       |          |           |                  |12 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bitsRI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(967 bytes) |12      |          |        |           |           |         |          |
 * |       |          |           |                  |13 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(1143 bytes)|30      |          |        |           |           |         |          |
 * |       |          |           |                  |10 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL61   |10 Mhz    |           |1 Rx Ant          |6           |        |          |        |           |           |         |          |
 * |       |          |QPSK       |(MRC)             |(85 bytes)  |5       |          |        |           |           |         |          |
 * |       |          |           |                  |12 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |QPSK       |(MRC)             |(117 bytes) |6       |          |        |           |           |         |          |
 * |       |          |           |                  |13 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(453 bytes) |9       |          |        |           |           |         |          |
 * |       |          |           |                  |11 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(533 bytes) |10      |          |        |           |           |         |          |
 * |       |          |           |                  |10 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(645 bytes) |8       |          |        |           |           |         |          |
 * |       |          |           |                  |24 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(1095 bytes)|12      |          |        |           |           |         |          |
 * |       |          |           |                  |40 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL62   |10 Mhz    |           |1 Rx Ant          |4           |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(645 bytes) |12      |          |        |           |           |         |          |
 * |       |          |           |                  |55 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(1095 bytes)|12      |          |        |           |           |         |          |
 * |       |          |           |                  |5 bits CQI  |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(621 bytes) |10      |          |        |           |           |         |          |
 * |       |          |           |                  |11 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(1143 bytes)|16      |          |        |           |           |         |          |
 * |       |          |           |                  |12 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL63   |10 Mhz    |           |1 Rx Ant          |4           |        |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(261 bytes) |12      |          |        |           |           |         |          |
 * |       |          |           |                  |55 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(645 bytes) |12      |          |        |           |           |         |          |
 * |       |          |           |                  |5 bits CQI  |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(317 bytes) |10      |          |        |           |           |         |          |
 * |       |          |           |                  |11 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(621 bytes) |16      |          |        |           |           |         |          |
 * |       |          |           |                  |12 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL64   |10 Mhz    |           |1 Rx Ant          |4           |        |          |        |           |           |         |          |
 * |       |          |QPSK       |(MRC)             |(41 bytes)  |12      |          |        |           |           |         |          |
 * |       |          |           |                  |55 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |QPSK       |(MRC)             |(261 bytes) |12      |          |        |           |           |         |          |
 * |       |          |           |                  |5 bits CQI  |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit RI    |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |QPSK       |(MRC)             |(87 bytes)  |10      |          |        |           |           |         |          |
 * |       |          |           |                  |11 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |QPSK       |(MRC)             |(241 bytes) |16      |          |        |           |           |         |          |
 * |       |          |           |                  |12 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL65   |10 Mhz    |           |1 Rx Ant          |6           |        |          |        |           |           |         |          |
 * |       |          |QPSK       |(MRC)             |(2 bytes)   |1       |          |        |           |           |         |          |
 * |       |          |           |                  |10 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(18 bytes)  |1       |          |        |           |           |         |          |
 * |       |          |           |                  |11 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |0 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(51 bytes)  |1       |          |        |           |           |         |          |
 * |       |          |           |                  |11 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |0 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |QPSK       |(MRC)             |(405 bytes) |45      |          |        |           |           |         |          |
 * |       |          |           |                  |55 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |QPSK       |(MRC)             |(2 bytes)   |1       |          |        |           |           |         |          |
 * |       |          |           |                  |10 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |16 QAM     |(MRC)             |(18 bytes)  |1       |          |        |           |           |         |          |
 * |       |          |           |                  |11 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |0 bit ACK   |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL66   |10 Mhz    |           |2 Rx Ant          |1           |        |8         |        |           |1          |         |          |
 * |       |          |64 QAM     |                  |(3662 bytes)|40      |Format 1  |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1  |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1a |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1a |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 2  |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 2  |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 2  |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 2  |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL67   |10 Mhz    |           |2 Rx Ant          |1           |        |5         |        |           |1          |         |          |
 * |       |          |64 QAM     |                  |(3062 bytes)|40      |Format 1  |        |           |           |         |          |
 * |       |          |           |                  |32 bit CQI  |        |Format 1  |        |           |           |         |          |
 * |       |          |           |                  |2 bit ACK   |        |Format 1a |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 1a |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 2  |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL68   |10 Mhz    |           |2 Rx Ant          |4           |        |8         |        |           |1          |         |          |
 * |       |          |64 QAM     |                  |(935 bytes) |10      |Format 1  |        |           |           |         |          |
 * |       |          |64 QAM     |                  |(935 bytes) |10      |Format 1  |        |           |           |         |          |
 * |       |          |64 QAM     |                  |(935 bytes) |10      |Format 1  |        |           |           |         |          |
 * |       |          |64 QAM     |                  |(935 bytes) |10      |Format 1  |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 2  |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 2  |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 2  |        |           |           |         |          |
 * |       |          |           |                  |            |        |Format 2  |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL69   |10 Mhz    |           |2 Rx Ant          |4           |        |8         |        |           |1          |         |          |
 * |       |          |64 QAM     |                  |(935 bytes) |10      |Format 1  |        |           |           |         |          |
 * |       |          |           |                  |11 bit CQI  |        |Format 1  |        |           |           |         |          |
 * |       |          |           |                  |2 bit RI    |        |Format 1  |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |Format 1  |        |           |           |         |          |
 * |       |          |64 QAM     |                  |(935 bytes) |10      |Format 1  |        |           |           |         |          |
 * |       |          |           |                  |11 bit CQI  |        |Format 2  |        |           |           |         |          |
 * |       |          |           |                  |2 bit RI    |        |Format 2  |        |           |           |         |          |
 * |       |          |           |                  |1 bit ACK   |        |Format 2  |        |           |           |         |          |
 * |       |          |64 QAM     |                  |(935 bytes) |10      |Format 2  |        |           |           |         |          |
 * |       |          |64 QAM     |                  |(935 bytes) |10      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL70   |10 Mhz    |           |2 Rx Ant          |4           |        |          |        |           |1          |         |          |
 * |       |          |64 QAM     |                  |(1095 bytes)|12      |          |        |           |           |         |          |
 * |       |          |           |                  |2 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |                  |(1836 bytes)|20      |          |        |           |           |         |          |
 * |       |          |           |                  |2 bit ACK   |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |                  |(935  bytes)|10      |          |        |           |           |         |          |
 * |       |          |64 QAM     |                  |(185  bytes)|2       |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL71   |20 Mhz    |           |1 Rx Ant          |4           |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(1335 bytes)|25      |          |        |           |           |         |          |
 * |       |          |           |                  |44 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(1335 bytes)|25      |          |        |           |           |         |          |
 * |       |          |           |                  |44 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(1335 bytes)|25      |          |        |           |           |         |          |
 * |       |          |           |                  |44 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(1335 bytes)|25      |          |        |           |           |         |          |
 * |       |          |           |                  |44 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL72   |10 Mhz    |           |1 Rx Ant          |4           |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(807 bytes) |15      |          |        |           |           |         |          |
 * |       |          |           |                  |32 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(807 bytes) |15      |          |        |           |           |         |          |
 * |       |          |           |                  |32 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(775 bytes) |10      |          |        |           |           |         |          |
 * |       |          |           |                  |28 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * |       |          |64 QAM     |(MRC)             |(775 bytes) |10      |          |        |           |           |         |          |
 * |       |          |           |                  |28 bits CQI |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits RI   |        |          |        |           |           |         |          |
 * |       |          |           |                  |2 bits ACK  |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL75   |10 Mhz    |           |2 Rx Ant          |1           |        |          |        |           |           |600Hz    |          |
 * |       |          |64 QAM     |                  |(4587 bytes)|50      |          |        |           |           |Freq Ofs |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL78   |10 Mhz    |           |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |Reservd|          |QPSK       |                  |( 217 bytes)|10      |          |        |           |           |         |          |
 * |       |          |           |                  |1 IRC interf|        |          |        |           |           |         |          |
 * |       |          |QPSK       |                  |( 217 bytes)|10      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL79   |10 Mhz    |           |2 Rx Ant          |1           |        |          |        |           |           |         |          |
 * |Reservd|          |QPSK       |                  |( 217 bytes)|10      |          |        |           |           |         |          |
 * |       |          |           |                  |1 IRC interf|        |          |        |           |           |         |          |
 * |       |          |QPSK       |                  |( 217 bytes)|10      |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL92   |20 Mhz    |16 QAM     |2 Rx Ant          |1           |        |          |        |           |1          |         |          |
 * |       |          |           |(MRC)             |(751 bytes) |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * |UL93   |5 Mhz     |16 QAM     |2 Rx Ant          |            |        |          |        |           |1          |         |          |
 * |       |          |           |(MRC)             |            |        |          |        |           |           |         |          |
 * ------------------------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 *
 * <BR/><BR/>
 *
 *
 * \section phy_be_tests_c 4.3  FD Test Suite
 * <BR/>
 * These are Full Duplextests that are run using the Lte_TestFD application in all 4 modes (C-Only, Ceva Sim and Hardware [LteProj and LteRT]).
 * There are batch files that look like <B><I>rt_fd_test*.bat</I></B>
 * (where '*' is the test number defined in table below) which have been configured to the specifications listed in table
 * below and can be used to run the tests.
 * <BR/><BR/><BR/>
 *
 * TABLE_START
 * ------------------------------------------------------------------------------------
 * |Test   |Bandwidth |Number of         |DL Test      |UL Test            |RT Mode   |
 * |Number |          |Antennas          |Option       |Option             |TTI       |
 * ------------------------------------------------------------------------------------
 * |FD1    |10 MHz    |1 Tx Antenna      |\ref DL31    |\ref UL31          |          |
 * |       |          |1 Rx Antenna      |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD2    |20 MHz    |2 Tx Antennas     |\ref DL35    |\ref UL35          |          |
 * |       |          |2 Rx Antennas     |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD3    |10 MHz    |2 Tx Antennas     |\ref DL5     |\ref UL2           |          |
 * |       |          |2 Rx Antennas     |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD4    |20 MHz    |1 Tx Antenna      |\ref DL15    |\ref UL33          |          |
 * |       |          |1 Rx Antenna      |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD5    |10 MHz    |1 Tx Antenna      |\ref DL22    |\ref UL22          |          |
 * |       |          |2 Rx Antenna      |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD6    |10 MHz    |2 Tx Antenna      |\ref DL23    |\ref UL22          |          |
 * |       |          |2 Rx Antenna      |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD7    |10 MHz    |2 Tx Antenna      |\ref DL24    |\ref UL22          |          |
 * |       |          |2 Rx Antenna      |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD36   |20 MHz    |2 Tx Antenna      |\ref DL36    |\ref UL36          |          |
 * |       |          |2 Rx Antenna      |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD37   |20 MHz    |2 Tx Antenna      |\ref DL37    |\ref UL37          |          |
 * |       |          |2 Rx Antenna      |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD38   |20 MHz    |2 Tx Antenna      |\ref DL38    |\ref UL38          |          |
 * |       |          |2 Rx Antenna      |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD39   |20 MHz    |2 Tx Antenna      |\ref DL39    |\ref UL39          |          |
 * |       |          |2 Rx Antenna      |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD43   |20 MHz    |2 Tx Antenna      |\ref DL39    |\ref UL43          |          |
 * |       |          |2 Rx Antenna      |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD44   |20 MHz    |2 Tx Antenna      |\ref DL36    |\ref UL44          |          |
 * |       |          |2 Rx Antenna      |             |                   |          |
 * ------------------------------------------------------------------------------------
 * |FD45   |10 MHz    |1 Tx Antenna      |\ref DL45    |\ref UL45          |          |
 * |       |          |2 Rx Antenna      |             |                   |          |
 * ------------------------------------------------------------------------------------
 * TABLE_END
 *
 * <BR/><BR/>
 *
 * </DIV>
 */

/**
 * \page phy_documentation 5.  Transcede&trade; Documentation and References
 *
 * <DIV style="text-align:justify;">
 *
 *
 * Below is the list of Transcede&trade; 40xx Related Documentation published to customers. The titles are always the same; the PN numbers
 * have the same portion in the following table and vary with different revisions by using the last revision identification code -A, -B, -C...
 * For example, we will have several revisions of datasheet published to customers:<BR/><BR/>
 * <B>Title</B>: M840xx Transcede&trade; Data Sheet<BR/>
 * <B>PN</B>:&nbsp;&nbsp;840xx-DSH-001-A (for revision A),<BR/>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;840xx-DSH-001-B (for revision B),<BR/>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;840xx-DSH-001-C (for revision C),<BR/>
 * <BR/> <BR/>
 *
 * <B style="font-size:15px;">Transcede&trade; 40xx Related Documentation</B><BR/><BR/>
 *
 * TABLE_START
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |Title                                                       | Part Number   | Description                                                                                                                                  |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |M840xx Transcede&trade; Product Brief                       | 840xx-BRF-001 |Provides device marketing brief.                                                                                                              |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |M840xx Transcede&trade; Data Sheet                          | 840xx-DSH-001 |Describes the hardware features, characteristics, pinout, power requirements, interfaces, and so on.                                          |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |M840xx Transcede&trade; Hardware Requirements Specification | 840xx-HWG-002 |Describes board design requirements specification.                                                                                            |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |M840xx Transcede&trade; Thermal Application Considerations  | 840xx-HWG-004 |Provides board thermal design considerations.                                                                                                 |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |M840xx Transcede&trade; Development Board and Design        | 840xx-EVMD-001|Describes layout and configurations of the development board and features and operations of the design kit.                                   |
 * |Kit User Guide                                              |               |                                                                                                                                              |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |M840xx Transcede&trade; Programming Guide                   | 840xx-SWG-001 | Provides comprehensive information for engineers who develop Transcede 40xx applications.                                                    |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |M840xx Transcede&trade; Software Architecture Specification | 840xx-SWG-002 | Provides an overview of application independent software architecture concepts, such as major functional blocks, data flow, control flow, and so on.|
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |M840xx Transcede&trade; PHY API Specification               | 840xx-SWG-004 | <B>Doxygen-generated PHY API for LTE</B> (this document)                                                                                     |
 * |Source Code Documentation (doxygen)                         |               |                                                                                                                                              |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |M840xx Transcede&trade; Performance Specifications          | 840xx-SWG-007 | Provides device operating performance data.                                                                                                  |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |M840xx Transcede&trade; Reference Application Product       | 840xx-SWG-009 | Provides a library of c-callable functions which run on, and are optimized for the Transcede 40xx SoC and demonstrates the capabilities      |
 * |Specification - Reference PHY for LTE Base Station          |               | of that SoC for implementing the highest throughput modes and lowest latency modes of the related standards.                                 |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |M840xx Transcede&trade; Release Notes x.y                   | 840xx-REL-001 | Customer software release notes. The release notes document is updated and available with every code delivery from Mindspeed.                      |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * TABLE_END
 * <BR/><BR/>
 *
 * </DIV>
 */

/*
 * TODO:
 * - Compile tools
 * - More test coverage
 * - Documentation links
 * - Projects in Ceva and ARM
 * - What to compile for what?
 * - Clean up compile flags
 * - Clean up TCB Example in TCB Wrapper Section
 */
