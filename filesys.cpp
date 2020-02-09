#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "filesys.h"

FILEEVENT EtoFE(EVENTSNODE e)
{
	FILEEVENT fe;

	strncpy(fe.szName,e.szName,25);
	fe.uiID = e.uiID;
	fe.uiValue = e.uiValue;

	return fe;
}

EVENTSNODE FEtoE(FILEEVENT fe)
{
	EVENTSNODE e;
	e.szName = new char[25];
	strncpy(e.szName,fe.szName,25);
	e.uiID = fe.uiID;
	e.uiValue = (LVLFLAGS)fe.uiValue;

	return e;
}

FILEITEM ItoFI(ITEM i)
{
	FILEITEM fi;

	fi.efunction = (int)i.efunction;
	fi.ilevel = i.ilevel;
	fi.iquantity = i.iquantity;
	fi.istrength = i.istrength;
	memcpy((void*)fi.runelevels,(void*)i.srunes.level,sizeof(fi.runelevels));
	strcpy(fi.szname,i.szname);
	fi.uiID = i.uiID;

	return fi;
}

ITEM FItoI(FILEITEM fi)
{
	ITEM i;

	i.efunction = (FUNCTION)fi.efunction;
	i.ilevel = fi.ilevel;
	i.iquantity = fi.iquantity;
	i.istrength = fi.istrength;
	memcpy((void*)i.srunes.level,(void*)fi.runelevels,sizeof(fi.runelevels));
	strcpy(i.szname,fi.szname);
	i.uiID = fi.uiID;

	if(i.uiID <= SWORD)
		i = g_Items[(int)i.uiID];
	else
		i.levelup = 0;

	return i;
}

bool SaveFile(char *file,FILEOBJECT data)
{
	unsigned int x=0;
	std::ofstream out(file, std::ios::binary);

	if( out )
	{
		data.header.size_of_events = sizeof(FILEEVENT)*data.footer.sizeof_event_array+sizeof(unsigned int);
		data.header.size_of_inv = sizeof(FILEITEM)*data.footer.sizeof_item_array+sizeof(unsigned int);
		strcpy(data.header.signature,"FL4");
		data.header.offset_to_stats = sizeof(data.header)+1;
		data.header.offset_to_events = sizeof(data.header)+
			sizeof(data.body)+1;
		data.header.offset_to_inv = sizeof(data.header)+
			sizeof(data.body)+sizeof(unsigned int) +
			data.footer.sizeof_event_array * sizeof(FILEEVENT)+1;

		out.write((const char*)&data.header,sizeof(data.header));
		out.write((const char*)&data.body,sizeof(data.body));
	
		out.write((const char*)&data.footer.sizeof_event_array,sizeof(unsigned int));
		for(x=0; x < data.footer.sizeof_event_array; x++)
		{
			out.write((const char*)&data.footer.events_data[x],sizeof(FILEEVENT));
		}

		out.write((const char*)&data.footer.sizeof_item_array,sizeof(unsigned int));
		for(x=0; x < data.footer.sizeof_item_array; x++)
		{
			out.write((const char*)&data.footer.items_data[x],sizeof(FILEITEM));
		}
	}
	else
	{
		return false;
	}

	out.close();

	return true;
}

FILEOBJECT *LoadFile(char *filename)
{
	unsigned int x=0;
	std::ifstream in;

	FILEOBJECT *file = new FILEOBJECT;

	in.open(filename, std::ios::binary | std::ios::_Nocreate);

	if( in )
	{
		in.read(( char*)&file->header,sizeof(file->header));
		in.read(( char*)&file->body,sizeof(file->body));
	
		in.read(( char*)&file->footer.sizeof_event_array,sizeof(unsigned int));
		file->footer.events_data = new FILEEVENT[file->footer.sizeof_event_array];
		for(x=0; x < file->footer.sizeof_event_array; x++)
		{
			in.read(( char*)&file->footer.events_data[x],sizeof(FILEEVENT));
		}

		in.read(( char*)&file->footer.sizeof_item_array,sizeof(unsigned int));
		file->footer.items_data = new FILEITEM[file->footer.sizeof_item_array];
		for(x=0; x < file->footer.sizeof_item_array; x++)
		{
			in.read(( char*)&file->footer.items_data[x],sizeof(FILEITEM));
		}
	}
	else
	{
		delete [] file;

		return NULL;
	}

	in.close();

	return file;
}