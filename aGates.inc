/*

 @@@@@@@@@@@@@@                @@@@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@@@@@@@@@
 @@@@@@@@@@@@@@                @@@@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@@@@@@@@@
 @@@@@@@@@@@@@@                @@@@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@@@@@@@@@
 @@@@@@  @@@@@@                @@@@@@         @@@@@@  @@@@@@     @@@@@@     @@@@@@         @@@@@@
 @@@@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@         @@@@@@@@@@@@@@     @@@@@@     @@@@@@@@@@@@@@ @@@@@@@@@@@@@@
 @@@@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@  @@@@@@ @@@@@@@@@@@@@@     @@@@@@     @@@@@@@@@@@@@@ @@@@@@@@@@@@@@
 @@@@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@  @@@@@@ @@@@@@@@@@@@@@     @@@@@@     @@@@@@@@@@@@@@ @@@@@@@@@@@@@@
 @@@@@@  @@@@@@                @@@@@@  @@@@@@ @@@@@@  @@@@@@     @@@@@@     @@@@@@                 @@@@@@
 @@@@@@  @@@@@@                @@@@@@@@@@@@@@ @@@@@@  @@@@@@     @@@@@@     @@@@@@@@@@@@@@ @@@@@@@@@@@@@@
 @@@@@@  @@@@@@                @@@@@@@@@@@@@@ @@@@@@  @@@@@@     @@@@@@     @@@@@@@@@@@@@@ @@@@@@@@@@@@@@
 @@@@@@  @@@@@@                @@@@@@@@@@@@@@ @@@@@@  @@@@@@     @@@@@@     @@@@@@@@@@@@@@ @@@@@@@@@@@@@@
 
						by ACE ABHISHEK
						
-> https://github.com/AceAbhishekOfficial
-> Read Instructions from github or https://www.burgershot.gg/


*/




#include <a_samp>
#include <streamer>

#define MAX_GATES 100

#define GATE_STATUS_CLOSED 0
#define GATE_STATUS_OPEN 1
#define GATE_STATUS_OPENING 2
#define GATE_STATUS_CLOSING 4
#define GATE_STATUS_DEV -1
#define GATE_STATUS_NOTCREATED -2


main()
{
	print("\n----------------------------------");
	print("\n---------ACE GATES LOADED----------");
	print("\n----------Version 1.0.0------------");
	print("\n-----------------------------------\n");

 #if defined aGates_main
        aGates_main();
    #endif
    return 1;
}

#if defined _ALS_main
    #undef main
#else
    #define _ALS_main
#endif
#define main aGates_main
#if defined aGates_main
    forward aGates_main();
#endif

enum ginfo
{
	gObjid,
	gAreaid,
	gStatus,

	Float:gOX,
	Float:gOY,
	Float:gOZ,
	Float:gORX,
	Float:gORY,
	Float:gORZ,

	Float:gCX,
	Float:gCY,
	Float:gCZ,
	Float:gCRX,
	Float:gCRY,
	Float:gCRZ,

	Float:gSpeed,
	bool:gAuto,
	bool:used,
}
new GateInfo[MAX_GATES][ginfo];

new AreaGate[500];

/////////////////////////////////////////////// Stock /////////////////////////////////////////////////////
stock GetGateID(objectid)
{
    	for(new a=0;a<MAX_GATES;a++)
	    if(GateInfo[a][used])
	    {
	        if(GateInfo[a][gObjid]==objectid)
	        return a;
     	}
     	return -1;
}
/////////////////////////////////////////////// FUNCTIONS ///////////////////////////////////////////////////

forward CreateGate(objid,worldid,interiorid,playerid,bool:autoOpen, Float:speed);
public CreateGate(objid,worldid,interiorid,playerid,bool:autoOpen, Float:speed)
{

	for(new a=0;a<MAX_GATES;a++)
	    if(!GateInfo[a][used])
	    {
	        new temp = CreateDynamicObject(objid,-1000.0,-1000.0,-1000.0, 0.0,0.0,0.0, worldid,interiorid,playerid,300.0,300.0);
			GateInfo[a][gObjid]=temp;
			GateInfo[a][gAuto]=autoOpen;
			GateInfo[a][used]=true;
			GateInfo[a][gSpeed]=speed;
			GateInfo[a][gStatus]=GATE_STATUS_DEV;
			CallRemoteFunction("OnGateCreation","i",a);
	        return a;
	    }
	return -1;
}
forward ConfigOpenState(gateid, Float:x,  Float:y,  Float:z,  Float:rx,  Float:ry,  Float:rz);
public ConfigOpenState(gateid, Float:x,  Float:y,  Float:z,  Float:rx,  Float:ry,  Float:rz)
{
    if(!IsValidGateID(gateid)) return 0;
	GateInfo[gateid][gOX]=x;
	GateInfo[gateid][gOY]=y;
	GateInfo[gateid][gOZ]=z;
	GateInfo[gateid][gORX]=rx;
	GateInfo[gateid][gORY]=ry;
	GateInfo[gateid][gORZ]=rz;
	return 1;
}
forward ConfigCloseState(gateid, Float:x,  Float:y,  Float:z,  Float:rx,  Float:ry,  Float:rz);
public ConfigCloseState(gateid, Float:x,  Float:y,  Float:z,  Float:rx,  Float:ry,  Float:rz)
{
    if(!IsValidGateID(gateid)) return 0;
	GateInfo[gateid][gCX]=x;
	GateInfo[gateid][gCY]=y;
	GateInfo[gateid][gCZ]=z;
	GateInfo[gateid][gCRX]=rx;
	GateInfo[gateid][gCRY]=ry;
	GateInfo[gateid][gCRZ]=rz;
	SetDynamicObjectPos(GateInfo[gateid][gObjid], x, y,z);
	SetDynamicObjectRot(GateInfo[gateid][gObjid], rx, ry,rz);
	GateInfo[gateid][gStatus]=GATE_STATUS_CLOSED;
	return 1;
}
forward SetGateTarget(gateid, Float:x, Float:y, Float:r);
public SetGateTarget(gateid, Float:x, Float:y, Float:r)
{
    if(!IsValidGateID(gateid)) return 0;
	new areaid=CreateDynamicCircle(x, y, r, -1,  -1,  -1);
	GateInfo[gateid][gAreaid] = areaid;
	AreaGate[areaid]=gateid;
	return 1;
}
forward IsValidGateID(gateid);
public IsValidGateID(gateid)
{
	if(gateid>=MAX_GATES || gateid<0) return 0;
	if(!GateInfo[gateid][used]) return 0;
	return 1;
}
forward OpenGate(gateid);
public OpenGate(gateid)
{
    if(!IsValidGateID(gateid)) return 0;
	if(GateInfo[gateid][gStatus]!=GATE_STATUS_CLOSED) return 0;
    MoveDynamicObject(GateInfo[gateid][gObjid],GateInfo[gateid][gOX],GateInfo[gateid][gOY],GateInfo[gateid][gOZ],GateInfo[gateid][gSpeed],GateInfo[gateid][gORX],GateInfo[gateid][gORY],GateInfo[gateid][gORZ]);
	GateInfo[gateid][gStatus]=GATE_STATUS_OPENING;
	return 1;
}
forward CloseGate(gateid);
public CloseGate(gateid)
{
    if(!IsValidGateID(gateid)) return 0;
	if(GateInfo[gateid][gStatus]!=GATE_STATUS_OPEN) return 0;
    MoveDynamicObject(GateInfo[gateid][gObjid],GateInfo[gateid][gCX],GateInfo[gateid][gCY],GateInfo[gateid][gCZ],GateInfo[gateid][gSpeed],GateInfo[gateid][gCRX],GateInfo[gateid][gCRY],GateInfo[gateid][gCRZ]);
	GateInfo[gateid][gStatus]=GATE_STATUS_CLOSING;
	return 1;
}
forward DestroyGate(gateid);
public DestroyGate(gateid)
{
    if(!IsValidGateID(gateid)) return 0;
	DestroyDynamicObject(GateInfo[gateid][gObjid]);
	GateInfo[gateid][used]=false;
	return 1;
}
/////////////////////////////////////////////// CALLBACKS USED ////////////////////////////////////////////////////////////



public OnPlayerEnterDynamicArea(playerid, areaid)
{
	new gateid = AreaGate[areaid];
	if(GateInfo[gateid][gAuto] && GateInfo[gateid][gStatus] == GATE_STATUS_CLOSED)
	OpenGate(gateid);
	if(!GateInfo[gateid][gAuto]  && GateInfo[gateid][gStatus] == GATE_STATUS_CLOSED)
	if(CallLocalFunction("OnPlayerRequestGate","ii",playerid,gateid))
	OpenGate(gateid);
 	#if defined aGates_OnPlayerEnterDynamicArea
        aGates_OnPlayerEnterDynamicArea(playerid, areaid);
    #endif
    return 1;
}

#if defined _ALS_OnPlayerEnterDynamicArea
    #undef OnPlayerEnterDynamicArea
#else
    #define _ALS_OnPlayerEnterDynamicArea
#endif
#define OnPlayerEnterDynamicArea aGates_OnPlayerEnterDynamicArea
#if defined aGates_OnPlayerEnterDynamicArea
    forward aGates_OnPlayerEnterDynamicArea(playerid, areaid);
#endif


public OnDynamicObjectMoved(objectid)
{
	new gateid = GetGateID(objectid);
	if(gateid==-1)
	return 1;
	if(GateInfo[gateid][gStatus]==GATE_STATUS_OPENING)
	{
		GateInfo[gateid][gStatus]=GATE_STATUS_OPEN;
		CallLocalFunction("OnGateOpen","i",gateid);
		//making sure gate is opened , preventing bugs
        SetDynamicObjectPos(GateInfo[gateid][gObjid], GateInfo[gateid][gOX], GateInfo[gateid][gOY],GateInfo[gateid][gOZ]);
		SetDynamicObjectRot(GateInfo[gateid][gObjid], GateInfo[gateid][gORX], GateInfo[gateid][gORY],GateInfo[gateid][gORZ]);
	}
	else if(GateInfo[gateid][gStatus]==GATE_STATUS_CLOSING)
	{
		GateInfo[gateid][gStatus]=GATE_STATUS_CLOSED;
		CallLocalFunction("OnGateClose","i",gateid);
 		//making sure gate is closed , preventing bugs
        SetDynamicObjectPos(GateInfo[gateid][gObjid], GateInfo[gateid][gCX], GateInfo[gateid][gCY],GateInfo[gateid][gCZ]);
		SetDynamicObjectRot(GateInfo[gateid][gObjid], GateInfo[gateid][gCRX], GateInfo[gateid][gCRY],GateInfo[gateid][gCRZ]);
	}
 #if defined aGates_OnDynamicObjectMoved
        aGates_OnDynamicObjectMoved(objectid);
    #endif
    return 1;
}

#if defined _ALS_OnDynamicObjectMoved
    #undef OnDynamicObjectMoved
#else
    #define _ALS_OnDynamicObjectMoved
#endif
#define OnDynamicObjectMoved aGates_OnDynamicObjectMoved
#if defined aGates_OnDynamicObjectMoved
    forward aGates_OnDynamicObjectMoved(objectid);
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


