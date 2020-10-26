# ACE GATES

Automatic Gates for SAMP/open.mp , more efficeint than anyother automatic gates

## Features

- Only ***Streamer*** plugin required.
- No ***timers*** or complex looping.
- ***OnPlayerRequestGate()*** called only time after player requests gate, not after every second if player is near gate.
- Personalisbale for ***players***, ***interirors*** and ***Virtual worlds***.
- fast and efficient
-  ***Open Source***

## Installation
Just paste `aGates.inc` in "pawno\includes" and add ***#include <agates>***

## Functions
### CreateGate(modelid, worldid, interiorid, playerid, bool:autoOpen, Float:speed) 
Create Automatic gate and returns the id of gate

| parameters | specidies |
|------------|-----------|
|modelid|model id of object to be sued as gate|
|worldid|The Virtual World id where gates is to be created (-1 for all)|
|interiorid|The interior id where gates is to be created (-1 for all)|
|playerid|The playerid id for which gates is to be created (-1 for all)|
|autoOpen|If true then gate will be opend automatically , else ***OnPlayerRequestGate*** will be called|
|speed|Speed of opening nad closing gates|
***Returns*** : gateid , gateid will be used in futher functions and callbacks


### ConfigOpenState(gateid, x, y, z, rx, ry, rz)
Set position and rotation of gate when it will be open
| parameters | specidies |
|-----------|------------|
|gateid|id of gate whose Open Sate is to be set|
|x|x coordinate of gate when open|
|y|y coordinate of gate when open|
|z|z coordinate of gate when open|
|rx|x rotation of gate when open|
|ry|y rotation of gate when open|
|rz|z rotation of gate when open|
***Returns*** : 1 if sucess else not

### ConfigCloseState(gateid, x, y, z, rx, ry, rz)

Set position and rotation of gate when it will be closed, gate will be orginally closed

| parameters | specidies |
|-----------|------------|
|gateid|id of gate whose closed Sate is to be set|
|x|x coordinate of gate when closed|
|y|y coordinate of gate when closed|
|z|z coordinate of gate when closed|
|rx|x rotation of gate when closed|
|ry|y rotation of gate when closed|
|rz|z rotation of gate when closed|
***Returns*** : 1 if sucess else not
(note:gate will be created only after this function)

### SetGateTarget(gateid, x, y, r)
Set Triger point and radius

| parameters | specidies |
|-----------|------------|
|x|x coordinate of gate to trigger|
|y|y coordinate of gate to trigger|
|r|distance from where trigger should work|



### OpenGate(gateid)
Open the gate

### CloseGate(gateid)
Close the gate

### GateStatus(gateid)
***Returns*** gate status ( check defines )

### IsValidGateID(gateid)
***Returns*** 1 if gateid is valid, otherwise

### DestryoGate(gateid)
dedstroys the gate

## Defines

#define MAX_GATES 100
#define GATE_STATUS_CLOSED 0
#define GATE_STATUS_OPEN 1
#define GATE_STATUS_OPENING 2
#define GATE_STATUS_CLOSING 4
#define GATE_STATUS_DEV -1
#define GATE_STATUS_NOTCREATED -2

## Callbacks

### OnGateCreation(gateid)
Called when gate is created , with gateid

### OnPlayerRequestGate(playerid,gateid)

Called when player requests gate
return 1 will make gate to open
return 0 will make to close
(note: not called when Auto is true)

### OnGateOpen(gateid)
Called when gate is opend

### OnGateClose(gateid)
Called when gate is closed


## Example Code :
```

#define TEAM_COP 2

new CopGate;

public OnGameModeInit()
{
	//other stuff

	CopGate = CreateGate(19912,-1,-1,-1,false, 10.0);
	ConfigOpenState(CopGate, 1592.6243, -1637.7130, 15.1031 , 0.0,0.0,0.0 );
	ConfigCloseState(CopGate, 1603.9211, -1637.7130, 15.1031, 0.0,0.0,0.0);
	SetGateTarget(CopGate, 1587.9806, -1637.1218, , 10.6999);
		
	//other stuff
}

forward OnPlayerRequestGate(playerid, gateid);
public OnPlayerRequestGate(playerid, gateid)
{
	if(gateid == Streetgate && GetPlayerTeam(playerid)==TEAM_COP)
	{
		
		 return 1;
	}
	SetTimerEx("MakeCloseGate",3000,false,"i",gateid); // timer to close gate after 3 seconds
	return 0;
	
	
}

forward MakeCloseGate(gateid);
public MakeCloseGate(gateid)
{
	CloseGate(gateid);	// close gate
	reutnr 1;
}

```
