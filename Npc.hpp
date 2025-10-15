#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include <string>
#include <list>
#include <map>
#include "wx/string.h"
#include "wx/arrstr.h"
#include "wx/stdpaths.h"
#include "wx/filename.h"
#include "wx/textfile.h"
#include "wx/cmdproc.h"

#include "AsfDecode.hpp"
#include "ListDefHelper.h"

struct NpcItem
{
	std::map<wxString, wxVariant> KeyValues;
    ImageDecode *NpcStand; //Point to GameImageList item, don't free

    long MapX()
    {
    	if(KeyValues.count(wxT("MapX")) > 0)
		{
			return KeyValues[wxT("MapX")].GetInteger();
		}
		return 0;
    }

    long MapY()
    {
    	if(KeyValues.count(wxT("MapY")) > 0)
		{
			return KeyValues[wxT("MapY")].GetInteger();
		}
		return 0;
    }

    void SetMapX(long x)
    {
    	KeyValues[wxT("MapX")] = x;
    }

    void SetMapY(long y)
    {
    	KeyValues[wxT("MapY")] = y;
    }

    wxString GetString(const wxString& key)
    {
    	if(KeyValues.count(key) > 0)
		{
			return KeyValues[key].GetString();
		}
		return wxT("");
    }

    long GetInteger(const wxString& key)
    {
    	if(KeyValues.count(key) > 0)
		{
			return KeyValues[key].GetInteger();
		}
		return 0;
    }

    void SetValue(const wxString&key, const wxVariant& value)
    {
    	KeyValues[key] = value;
    }

	NpcItem() {NpcStand = NULL;}
    void CopyFrom(const NpcItem *item);
};
struct ObjItem
{
    std::map<wxString, wxVariant> KeyValues;
    ImageDecode *ObjCommon; //Point to AsfObjImgList item, don't free

    long MapX()
    {
    	if(KeyValues.count(wxT("MapX")) > 0)
		{
			return KeyValues[wxT("MapX")].GetInteger();
		}
		return 0;
    }

    long MapY()
    {
    	if(KeyValues.count(wxT("MapY")) > 0)
		{
			return KeyValues[wxT("MapY")].GetInteger();
		}
		return 0;
    }

    void SetMapX(long x)
    {
    	KeyValues[wxT("MapX")] = x;
    }

    void SetMapY(long y)
    {
    	KeyValues[wxT("MapY")] = y;
    }

	long GetOffX()
    {
		long offx = GetInteger(wxT("OffX"));
		return offx == -1 ? 0 : offx;
	}
    long GetOffY()
    {
    	long offy = GetInteger(wxT("OffY"));
    	return offy == -1 ? 0 : offy;
	}

	void SetOffX(long x)
    {
    	KeyValues[wxT("OffX")] = x;
    }

    void SetOffY(long y)
    {
    	KeyValues[wxT("OffY")] = y;
    }

    wxString GetString(const wxString& key)
    {
    	if(KeyValues.count(key) > 0)
		{
			return KeyValues[key].GetString();
		}
		return wxT("");
    }

    long GetInteger(const wxString& key)
    {
    	if(KeyValues.count(key) > 0)
		{
			return KeyValues[key].GetInteger();
		}
		return 0;
    }

	void SetValue(const wxString&key, const wxVariant& value)
    {
    	KeyValues[key] = value;
    }

	ObjItem(){ObjCommon = NULL;}
    void CopyFrom(const ObjItem *item);


};
struct GameImage
{
    wxString path;
    ImageDecode *imgdec;
};

typedef std::list<GameImage*> GameImageList;
typedef std::list<GameImage*>::iterator GameImageListIterator;
// if in , outimg is the img data
bool IsImageFileIn(wxString path, GameImageList *list, ImageDecode **outimg);
// Add img to list
GameImage* AddImg(GameImageList &list, const wxString &imgFilePath);
//delete list item GameImage*, GameImage::imgdec, and clear list
void FreeGameImageList(GameImageList *list);

// find [stand] img file in npcres ini file
std::string FindImgInIni(const std::string FilePath, const std::string match);
//Find imgdecoder in NpcItem or ObjItem or both ini res file
void FindAndBufferImage(const wxString &exepath, NpcItem *npcitem, ObjItem *objitem, GameImageList *asflist);
// find [stand] asf and buffer its data
//exepath : the end contain path seprator
//imgdec : out  , get a point of AsfDecode data
//asflist : AsfDecode manager list
bool FindAndBufferImage(const wxString &exepath,
                      const wxString &inifilename,
                      const wxString &match,
                      ImageDecode **imgdec,
                      GameImageList *imglist = NULL);

// init NpcItem to default vaule
void InitNpcItem(NpcItem *item);
void InitObjItem(ObjItem *item);
// Read a npc ini file and initializing item
//exepath : the end contain path seprator
//list : if not null, npc's asfdata is add to list
bool ReadIni(const wxString &exepath,
             const wxString &filePath,
             NpcItem *npcitem = NULL,
             ObjItem *objitem = NULL,
             GameImageList *list = NULL);
bool SaveIni(const wxString &filePath, NpcItem *npcitem = NULL, ObjItem *objitem = NULL, ListDefHelper *listDefHelper = NULL);
void SaveNpcItem(wxTextFile &file, NpcItem *item, ListDefHelper *listDefHelper);
void SaveObjItem(wxTextFile &file, ObjItem *item, ListDefHelper *listDefHelper);
void AssignNpcItem(const wxString &name, const wxString &value, long n_value, NpcItem *item);
void AssignObjItem(const wxString &name, const wxString &value, long n_value, ObjItem *item);
bool GetNameValue(const wxString &line, wxString &name, wxString &value, long *n_value);

template<class T>
class ItemList
{
public:
	typedef typename std::list<T>::iterator iterator;
    ItemList()
    {
        exepath = wxStandardPaths::Get().GetExecutablePath();
        exepath = wxFileName::FileName(exepath).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    }
    ~ItemList()
    {
        FreeData();
    }

    T GetItem(int index)
    {
        if(index < 0 || index >= (int)m_list.size()) return T(NULL);
        typename std::list<T>::iterator it = m_list.begin();
        for(int i = 0; i < index; i++, it++);
        return *it;
    }

    int GetIndex(T item)
    {
    	int index = 0;
        for(typename std::list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it, index++)
        {
        	if(*it == item) return index;
        }
        return -1;
    }

	//Returned array must deleted use delete[]
    T* GetAllItem()
    {
    	size_t num = m_list.size();
    	T *items = new T[num + 1];
    	items[num] = NULL;
    	long pos = 0;
    	for(typename std::list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it)
		{
			items[pos++] = *it;
		}
		return items;
    }

    T GetItem(long mapx, long mapy, long *index = NULL)
    {
        long pos = 0;
        for(typename std::list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it, pos++)
        {
            if((*it)->MapX() == mapx && (*it)->MapY() == mapy)
            {
                if(index != NULL) (*index) = pos;
                return *it;
            }
        }
        return T(NULL);
    }

    void InsertItem(int pos, T item)
    {
    	if(pos == 0)
		{
			m_list.insert(m_list.begin(), item);
		}
		else
		{
			int i = 0;
			bool inserted = false;
			auto it = m_list.begin();
			for(; it != m_list.end();)
			{
				if(i == pos)
				{
					m_list.insert(it, item);
					inserted = true;
					break;
				}
				i++;
				it++;
			}

			if(!inserted)
			{
				//insert at end
				m_list.insert(it, item);
			}
		}
    }

    void AddItem(T item)
    {
        if(item == NULL) return;
        m_list.push_back(item);
    }

    void DeleteBack()
    {
    	if(m_list.size() > 0)
		{
			delete m_list.back();
			m_list.pop_back();
		}
    }

    void DeleteItem(long mapx, long mapy)
    {
        for(typename std::list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it)
        {
            if((*it)->MapX() == mapx && (*it)->MapY() == mapy)
            {
                delete *it;
                m_list.erase(it);
                return;
            }
        }
    }
    void DeleteItem(int index)
    {
    	if(index < 0 || index >= (int)m_list.size()) return;
        typename std::list<T>::iterator it = m_list.begin();
        for(int i = 0; i < index; i++, it++);
    	delete *it;
		m_list.erase(it);
    }

    int getCounts()
    {
        return m_list.size();
    }

    bool HasItem(long mapx, long mapy)
    {
        for(typename std::list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it)
        {
            if((*it)->MapX() == mapx && (*it)->MapY() == mapy)
            {
                return true;
            }
        }
        return false;
    }

    iterator begin()
    {
    	return m_list.begin();
    }

    iterator end()
    {
    	return m_list.end();
    }

    void Clear()
    {
        FreeData();
    }

    //clear list without deleting items in list.
    void ClearWithoutFreeData()
    {
		m_list.clear();
    }

    void HandoverOwnship(ItemList<T>& newOwner)
    {
		for(typename std::list<T>::iterator it = m_list.begin();
                it != m_list.end(); ++it)
        {
            newOwner.AddItem(*it);
        }
        m_list.clear();
    }

private:
    void FreeData()
    {
        if(m_list.empty()) return;
        for(typename std::list<T>::iterator it = m_list.begin();
                it != m_list.end(); ++it)
        {
            delete *it;
        }
        m_list.clear();
    }


    std::list<T> m_list;
    wxString exepath;
};

typedef ItemList<NpcItem*> NpcList;
bool NpcListImport(const wxString &exepath, const wxString &path, NpcList *list, GameImageList *asflist, wxCommandProcessor *cmdProc);
bool NpcListSave(const wxString path, const wxString mapName, NpcList *list, ListDefHelper *listDefHelper);

typedef ItemList<ObjItem*> ObjList;
bool ObjListImport(const wxString &exepath, const wxString &path, ObjList *list, GameImageList *asflist, wxCommandProcessor *cmdProc);
bool ObjListSave(const wxString path, const wxString mapName, ObjList *list, ListDefHelper *listDefHelper);

#endif // NPC_HPP_INCLUDED
