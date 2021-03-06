#define PARAMS 256

typedef enum {
   FIRST_COMMAND,
   H,COM,SPACE,
   Q,
   DON,DOF,TG,EN,DS,RES,SSC,ONC,SIM,
   DM,EM,
   RHST,RST,RDT,RSC,
   DPRAM,VRS,RAD,DPT,
   AGAIN,AREL,
   REC,INT,AUX,EVH,CLK,
   TR,L,CLR,UT,
   OBJ,PAR,
   ML,MT,MO,MS,MI,MC,MD,
   DAL,DEF,DPPM,DEC,
   WAIT,CON,SIG,TEL,
   BMC,XMC,SMC,
   LAST_COMMAND
 } Command;

char * commands[LAST_COMMAND] = {
   "?",
   "h",";"," ",
   "q",
   "don","doff","tg8","en","ds","res","ssc","onc","sim",
   "dm","em",
   "rhst","rst","rdt","rsc",
   "dpram","vrs","rad","dpt",
   "again","arel",
   "rec","int","aux","evh","clk",
   "tr","l","clr","ut",
   "obj","par",
   "ml","mt","mo","ms","mi","mc","md",
   "dal","def","dppm","dec",
   "wait","con","sig","tel",
   "bmc","xmc","smc"
 };

char * hlptxt1 =
" q ------------------------ Quit program\n"
" res ---------------------- RESet module\n"
" en, ds <sync> ------------ ENable or DiSable current working module Sync/Async\n"
" tg8 <mod> <addr> <vect> <lev> <switch> <ssc> --- Set/Install the TG8 card\n"
" onc <mode> <mask> -------- Close mode setup (0=LEAVE,1=DISABLE,2=DIS&CLEAR,3=DIS.SYNCHRO)\n"
" sim <mask> --------------- Simulate the output pulses on the Tg8 card\n"
" ssc ---------------------- Set the SSC Header code\n"
" rst, rhst ---------------- Read Software or Hardware STatus\n"
" rad ---------------------- Read the current driver configuration\n"
" rdt ---------------------- Read current radio clock Date and Time\n"
" rsc ---------------------- Read current super cycle information\n"
" again <path> ------------- Install firmware AGAIN\n\n";

char * hlptxt2 =
" ut <file> ---------------- Download a new user table\n"
" def <n> <cnt> ------------ Display the User Table\n"
" rec <n> <cnt> ------------ Read recording table\n"
" int <n> <cnt> ------------ Read the interrupt table\n"
" aux ---------------------- Read the auxiliary table\n"
" evh <cnt> <frame> -------- Read the events history table\n"
" clk <cnt> ---------------- Read the clock table\n\n";

char * hlptxt3 =
" ml trigger control delay - Set the new action\n"
" mt <line> <trg> ---------- Modify trigger event\n"
" mo <line> <channel> ------ Modify channel number [1-8]\n"
" ms <line> <start> -------- Modify start mode (0=NORM,1=CHAIN,2=EXTRN)\n"
" mi <line> <intr> --------- Modify interrupt type (0=DSBL,1=OUT,2=VME,3=OUT+VME)\n"
" mc <line> <clk> ---------- Modify Clock (0=MS,1=CABLE,2=E1/IN,3=EXT2)\n"
" md <line> <delay> -------- Modify Delay\n"
" la <n1> <n2> ------------- List actions from user table\n"
" clr <n> <cnt> ------------ Clear a part of the user table\n"
" dm, em <line> <cnt> ------ Disable or Enable an User Table Line\n\n";

char * hlptxt4 =
" wait <event> <cnt> <step>- Wait for any event\n"
" sig <signum> ------------- Set the signal number for event reception\n\n";

char * hlptxt5 =
" obj id trigger control delay machine Gnum Gtype Gval dim - Set the timing object\n"
" par iden line sel val clk- Modify object's parameters (sel:1=state;2=delay)\n"
" con <obj> <mask> <cnt> --- Connect to lines of timing object\n"
" dppm <n> <cnt> ----------- Display the PPM info from the User Table\n\n";

char * hlptxt6 =
" bmc, xmc, <n> - smc ------ Build, eXecute, Show MACROs\n"
" don, doff ---------------- Set driver Debugging ON or OFf\n"
" tr ----------------------- Trace module firmware break point\n"
" dpram -------------------- Get the DPRAM content\n"
" dpt ---------------------- Test the DPRAM\n"
" vrs ---------------------- Get driver and firmware VeRSion\n"
" arel --------------------- RELoad the Action table\n"
" dal <n> <cnt> ------------ Display All actions\n"
" dec <n> <cnt> <chan> ----- Display actions for the given channel\n"
" tel <machine> <cnt> ------ Read telegram (1=LEP,2=SPS,3=CPS,4=PSB,5=LPI)\n";
;

#define HELPS 6
static char *hlptxts[HELPS];

/* eof tg8test.h */
