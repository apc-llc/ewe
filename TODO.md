Global TODO list
================

Electrocardio
=============
* Add (Boolean) material property that shows the de/repolarization status of the cells
* <s>Make stimulation parameters adjustable from outside (e.g.. via user-defined functions)</s>
* <s>Use fibre directions for anisotropic conductivities</s>
* Set appropriate parameter values for the anisotropic conductivity values, see e.g. \ref Potse2006 "Potse, 2006, Table I"
* Add option for getting an ion model's resting potential. - Currently, it is fixed in ElectrocardioIC


Electrocardio: Celltypes in the ion model
=========================================
This is German because it is a copy of an email that shall not be forgotten - sorry for that.

Beim Lesen in dem Bernus-Paper ist mir aufgefallen, dass die Zelltypen ja gar nicht wirklich a-priori enthalten sind, sondern im Prinzip künstlich im Nachhinein eingebaut wurden (Siehe den Abschnitt "AP Heterogenity" auf Seiten H2301f sowie Table 4).
Dahingehend verstehe ich jetzt auch dass es noch kein Interface zu den Zelltypen gibt.
Können wir da morgen mal drüber sprechen wie man das sinnvoll einbauen kann?

Was wir ja mittelfristig im Vergleich zu propag bräuchten wären ortsabhängige:
  * Wahl der Ionenmodelle
  * Wahl der Untertypen, i.e. Zelltypen
  * Leitfähigkeiten, i.e. Substances
Letzteres ist sicherlich einfach, die ersten beiden brauchen noch etwas Interface-Unterstützung im Ionmodel.h - Ich habe bisher keine gute Idee um sowas ausreichend generisch (gegenüber einem potentiellen Austausch des Ionenmodells) zu machen...

Notizen darüber, wo interessante Anregungen zu finden sind:
- Zelltypen:
   * bernus.h: 

                 char bernus_cname[BERNUS_NTYPES][20]= {"epic","mcel","endo","isch","rvmc"};

                 #define    BERNUS_EPIC 0     // LV+RV epicardial
                 #define    BERNUS_MCEL 1     // LV or LV+RV midmyocardial
                 #define    BERNUS_ENDO 2     // LV+RV endocardial
                 #define    BERNUS_ISCH 3     // LV+RV endocardial ischemic
                 #define    BERNUS_RVMC 4     // RV midmyocardial \cite{volders99,verkerk05}

                 Memtab[tr][BERNUS_ENDO][TC_EXP_X] = exp(-dt/tau_X);
                 Memtab[tr][BERNUS_EPIC][TC_EXP_X] = exp(-dt/tau_X);

                 Memtab[tr][BERNUS_MCEL][TC_INF_X] = 0.972 /(1.0+exp(-2.036-0.0834*vm));
                 tau_X_M = 380*a + 166*(1+tanh(0.558+0.0169*vm)) ;
                 Memtab[tr][BERNUS_MCEL][TC_EXP_X] = exp(-dt/tau_X_M);

                 Memtab[tr][BERNUS_RVMC][TC_INF_X] = 0.972 /(1.0+exp(-2.036-0.0834*vm));
                 tau_X_M = 380*a + 166*(1+tanh(0.558+0.0169*vm)) ;
                 Memtab[tr][BERNUS_RVMC][TC_EXP_X] = exp(-dt/tau_X_M);

   * tpx.web: 

                 char tpx_cname[TPX_NTYPES][20] = {"epic", "mcel", "endo", "rvmc", "rvepi", "type5", "type6"}

                 #define    TPX_EPIC 0     // LV+RV epicardial
                 #define    TPX_MCEL 1     // LV or LV+RV midmyocardial
                 #define    TPX_ENDO 2     // LV+RV endocardial
                 #define    TPX_RVMC 3     // RV midmyocardial
                 #define    TPX_RVEPI 4     // RV epicardial
                 #define    TPX_T5    5     // spare type, has endo characteristics by default
                 #define    TPX_T6    6     // systematic naming for spare types

                 see @<tabulate steady-state values and time constants@>+=

- Substances:
    * anatomy.h: 

                 #define set_default_substance(n,s,Sel,Set,Sil,Sit)  \
                 if(strcmp(prm_substance[n].name,"none") ==0) { \
                 prm_substance[n].name= s; \
                 prm_substance[n].sigma_el= Sel; \
                 prm_substance[n].sigma_et= Set; \
                 prm_substance[n].sigma_ec= -1.0; \
                 prm_substance[n].sigma_il= Sil; \
                 prm_substance[n].sigma_it= Sit; \
                 prm_substance[n].sigma_ic= -1.0; \
                 } \
                 set_default_substance(0,"void",0.0,0.0,0.0,0.0);
                 set_default_substance(1,"ventricle",3.0,1.2,3.0,0.3);
                 set_default_substance(2,"atrium",3.0,1.2,3.0,0.3);
                 set_default_substance(3,"inactive",2.0,2.0,0.0,0.0);
                 set_default_substance(4,"ischemic",3.0,1.2,3.0,0.3);
                 set_default_substance(5,"blood",6.0,6.0,0.0,0.0);
                 set_default_substance(6,"His bundle",3.0,1.2,3.0,0.3);
                 set_default_substance(7,"AV node",3.0,1.2,3.0,0.3); 

Reading spatially varying parameters from Exodus files
======================================================
For the same reason as above German again - sorry.
...brauchen wir für ortsabhängige
 * Wahl von Zelltypen
 * Leitfähigkeiten
 * evtl. Faserrichtungen
 * Auswahl der Stimulationsposition
 * Weitere Eigenschaften ??

Wird wohl funktionieren mit Hilfen von:
 * moose/framework/include/userobject/SolutionUserObject.h / moose/framework/src/userobject/SolutionUserObject.C
Zumindest stellt dieses Zugriff auf Variablen via
 * `Real pointValue(Real t, const Point & p, const std::string & var_name)`
 * `Real directValue(const Node * node, const std::string & var_name)`
 * `Real directValue(const Elem * elem, const std::string & var_name)`
zur Verfügung.

Beispiele zur Verwendung finden sich in
 * `moose/framework/src/auxkernels/SolutionAux.C`
 * `moose/framework/src/functions/SolutionFunction.C`

Fraglich bleibt: Wenn man ein komplettes System mit dem SolutionUserObject lädt, kann man dann auf die [Mesh] Section verzichten?

Function
========
 * <s>give a list of function names to PiecewiseParsedFunction instead of explicit function definitions in `functions` parameter to increase readability</s>
 * add a new function definition which is a sum of several spatial Gaussians (defined by a list of 3D-points) to simplify definition of
   several activation sites

CardiacFibresMaterial
=====================
* Find a good way to distinguish left and right ventricle elements in the computation of fibre directions. - Currently, CardiacThicknessParameter already supplies the necessary information (by means of the sign of e) but this is not interpreted correctly, yet.
* Ensure that the z-axis is always pointing from the apex to the left aortic valve commissure as in \ref Streeter1969 "Streeter, 1969, Figure 1"
* In fact, the \ref Streeter1969 construction is only valid for the left ventricle. For the right ventricle it could for example be repeated accordingly with appropriately tilted z-axis. Currently, left and right ventricle are not distinguished at all, see also comments in ewe_notes.pdf.
* Include regional fibre direction variability of up to 15 degrees \ref Karlon2000

CardiacMechanicsMaterial
========================
* <s>Couple to displacements through a coupled vector instead of individual disp_.. variables.</s>
    * <s>For details see http://mooseframework.org/wiki/Faq/#coupling-to-an-arbitrary-number-of-variables-back-to-top</s>
	* <s>A working example is found in CardiacMechanicsMaterial.C</s>
	* <s>All affected files can be found via `grepc -iR \"disp src/*`</s>
* Find out how Ta enters into the elastic energy
* Add material law from \ref Costa2001. Parameter values should be available in \ref Schmid2008 .

SecondDerivativeNewmark
=======================
* Check and rework Newmark integrator, especially think about using 
   * `declarePropertyOlder()`
   * `parameters.set<bool>("implicit") = true`
   * `kernel->isImplicit()`

CardiacIncompressibilityLagrangeMultiplier
==========================================
* Think about the Jacobian of this kernel

General
=======
* Clean formatting of all *.i files that are around
* <s>Find a better way for including references in doxygen comments, see `documentation_cleanup` branch</s>
* Further TODOs in the inline documentation, see the \ref todo "TODO list"
