///////////////////////////////////////////////////////////////////////////
// ComponentConfig.h, component configuration parameters.
//
// Initially generated by the component wizard. Feel free to manually edit.
///////////////////////////////////////////////////////////////////////////

// Component framework type
#define BON2COMPONENT

// COM UUID-s, names and progID
#define TYPELIB_UUID "A8FF03ED-C8FF-45E9-97E9-0389DCB772C9"
#define TYPELIB_NAME "MGA Interpreter TypeLibrary (CoDesign)"
#define COCLASS_UUID "D44ED2D8-9F64-403F-B08E-90F33426150A"
#define COCLASS_NAME "MGA Interpreter CoClass (CoDesign)"
#define COCLASS_PROGID "MGA.Interpreter.CoDesign"

#define COCLASS_UUID_EXPLODED1  0xd44ed2d8
#define COCLASS_UUID_EXPLODED2   0x9f64
#define COCLASS_UUID_EXPLODED3   0x403f
#define COCLASS_UUID_EXPLODED4   0xb0
#define COCLASS_UUID_EXPLODED5   0x8e
#define COCLASS_UUID_EXPLODED6   0x90
#define COCLASS_UUID_EXPLODED7   0xf3
#define COCLASS_UUID_EXPLODED8   0x34
#define COCLASS_UUID_EXPLODED9   0x26
#define COCLASS_UUID_EXPLODED10  0x15
#define COCLASS_UUID_EXPLODED11  0xa


// This name will appear in the popup window for interpreter selection.
#define COMPONENT_NAME _T("CoDesign")


// This text will appear in the toolbar icon tooltip and in the menu.
#define TOOLTIP_TEXT _T("CoDesign Interpreter")

// This macro determines the component type (addon vs. interpreter):
#define GME_ADDON
#define ADDON_EVENTMASK 0xe1faffff

// The name of the paradigm(s). The GME will use this component
// for this paradigm. Separate the names of paradigms by commas.
//#define PARADIGM_INDEPENDENT
#define PARADIGMS "AssortedxADL"

#define BON_ICON_SUPPORT

// #define BON_CUSTOM_TRANSACTIONS

#define REGISTER_SYSTEMWIDE