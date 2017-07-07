///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MapFrameBase.h"

///////////////////////////////////////////////////////////////////////////

MapFrameBase::MapFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_toolBar1 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTB_HORIZONTAL ); 
	m_tool7 = m_toolBar1->AddTool( ID_OPENMAP, wxT("tool"), wxICON( OPENFILE ), wxNullBitmap, wxITEM_NORMAL, wxT("打开地图..."), wxT("CTRL+O"), NULL ); 
	
	m_toolBar1->AddSeparator(); 
	
	m_toolBar1->AddSeparator(); 
	
	m_toolBar1->AddSeparator(); 
	
	m_Layer1 = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("图层1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Layer1->SetValue(true); 
	m_toolBar1->AddControl( m_Layer1 );
	m_Layer2 = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("图层2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Layer2->SetValue(true); 
	m_toolBar1->AddControl( m_Layer2 );
	m_Layer3 = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("图层3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Layer3->SetValue(true); 
	m_toolBar1->AddControl( m_Layer3 );
	m_toolBar1->AddSeparator(); 
	
	m_toolBar1->AddSeparator(); 
	
	m_Trap = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("陷阱"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_Trap );
	m_Barrer = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("障碍"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_Barrer );
	m_toolBar1->AddSeparator(); 
	
	m_toolBar1->AddSeparator(); 
	
	m_LayerTransparent = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("图层透明"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LayerTransparent->SetToolTip( wxT("使图层透明的地方透明，而不是黑色") );
	
	m_toolBar1->AddControl( m_LayerTransparent );
	m_toolBar1->Realize(); 
	
	bSizer2->Add( m_toolBar1, 1, wxEXPAND, 5 );
	
	m_ToolBarEdit = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTB_HORIZONTAL ); 
	m_tool5 = m_ToolBarEdit->AddTool( ID_NPCMODE, wxT("tool"), wxICON( NPC ), wxNullBitmap, wxITEM_CHECK, wxT("人物编辑模式（F2）"), wxEmptyString, NULL ); 
	
	m_tool6 = m_ToolBarEdit->AddTool( ID_OBJMODE, wxT("tool"), wxICON( OBJ ), wxNullBitmap, wxITEM_CHECK, wxT("物品编辑模式（F3）"), wxEmptyString, NULL ); 
	
	m_ToolBarEdit->AddSeparator(); 
	
	m_ToolPlace = m_ToolBarEdit->AddTool( ID_TOOLPLACE, wxT("tool"), wxICON( PLACE ), wxNullBitmap, wxITEM_CHECK, wxT("放置模式（P）"), wxEmptyString, NULL ); 
	
	m_ToolDelete = m_ToolBarEdit->AddTool( ID_TOOLDELETE, wxT("tool"), wxICON( DELETE ), wxNullBitmap, wxITEM_CHECK, wxT("删除模式（D）"), wxEmptyString, NULL ); 
	
	m_ToolEditAttribute = m_ToolBarEdit->AddTool( ID_EDITATTRIBUTE, wxT("tool"), wxICON( ATTRIBUTE ), wxNullBitmap, wxITEM_CHECK, wxT("属性设置模式（A）"), wxEmptyString, NULL ); 
	
	m_tool4 = m_ToolBarEdit->AddTool( ID_TOOLMOVE, wxT("tool"), wxICON( MOVE ), wxNullBitmap, wxITEM_CHECK, wxT("移动模式（M）"), wxEmptyString, NULL ); 
	
	m_ToolBarEdit->AddSeparator(); 
	
	m_tool8 = m_ToolBarEdit->AddTool( ID_SHOWNPC, wxT("tool"), wxICON( VIEWNPC ), wxNullBitmap, wxITEM_CHECK, wxT("显示人物（N）"), wxEmptyString, NULL ); 
	
	m_tool9 = m_ToolBarEdit->AddTool( ID_SHOWOBJ, wxT("tool"), wxICON( VIEWOBJ ), wxNullBitmap, wxITEM_CHECK, wxT("显示物品（O）"), wxEmptyString, NULL ); 
	
	m_ToolBarEdit->Realize(); 
	
	bSizer2->Add( m_ToolBarEdit, 0, wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	m_mapViewWindow = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_mapViewWindow->SetSashGravity( 1 );
	m_mapViewWindow->Connect( wxEVT_IDLE, wxIdleEventHandler( MapFrameBase::m_mapViewWindowOnIdle ), NULL, this );
	m_mapViewWindow->SetMinimumPaneSize( 1 );
	
	m_MapView = new wxPanel( m_mapViewWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_SideCtrl = new wxPanel( m_mapViewWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter4 = new wxSplitterWindow( m_SideCtrl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter4->Connect( wxEVT_IDLE, wxIdleEventHandler( MapFrameBase::m_splitter4OnIdle ), NULL, this );
	m_splitter4->SetMinimumPaneSize( 1 );
	
	m_MapControl = new wxPanel( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelList = new wxPanel( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	m_ListData = new wxNotebook( m_panelList, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel5 = new wxPanel( m_ListData, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_npcListCtrl = new wxListView( m_panel5, MYID_NPCLISTCTRL, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bSizer20->Add( m_npcListCtrl, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel5->SetSizer( bSizer20 );
	m_panel5->Layout();
	bSizer20->Fit( m_panel5 );
	m_ListData->AddPage( m_panel5, wxT("NPC"), true );
	m_panel6 = new wxPanel( m_ListData, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	m_objListCtrl = new wxListView( m_panel6, MYID_OBJLISTCTRL, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bSizer21->Add( m_objListCtrl, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel6->SetSizer( bSizer21 );
	m_panel6->Layout();
	bSizer21->Fit( m_panel6 );
	m_ListData->AddPage( m_panel6, wxT("OBJ"), false );
	
	bSizer19->Add( m_ListData, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panelList->SetSizer( bSizer19 );
	m_panelList->Layout();
	bSizer19->Fit( m_panelList );
	m_splitter4->SplitHorizontally( m_MapControl, m_panelList, 100 );
	bSizer6->Add( m_splitter4, 1, wxEXPAND, 5 );
	
	
	m_SideCtrl->SetSizer( bSizer6 );
	m_SideCtrl->Layout();
	bSizer6->Fit( m_SideCtrl );
	m_mapViewWindow->SplitVertically( m_MapView, m_SideCtrl, -200 );
	bSizer1->Add( m_mapViewWindow, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	m_StatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_menubar3 = new wxMenuBar( 0 );
	m_MenuFile = new wxMenu();
	wxMenuItem* m_menuItemOpenMap;
	m_menuItemOpenMap = new wxMenuItem( m_MenuFile, wxID_ANY, wxString( wxT("打开地图...") ) + wxT('\t') + wxT("CTRL+O"), wxEmptyString, wxITEM_NORMAL );
	m_MenuFile->Append( m_menuItemOpenMap );
	
	wxMenuItem* m_menuItemSavePng;
	m_menuItemSavePng = new wxMenuItem( m_MenuFile, wxID_ANY, wxString( wxT("存为PNG...") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuFile->Append( m_menuItemSavePng );
	
	m_MenuFile->AppendSeparator();
	
	wxMenuItem* m_menuItemUP;
	m_menuItemUP = new wxMenuItem( m_MenuFile, ID_MAPUP, wxString( wxT("地图上移\tUp") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuFile->Append( m_menuItemUP );
	
	wxMenuItem* m_menuItemDOWN;
	m_menuItemDOWN = new wxMenuItem( m_MenuFile, ID_MAPDOWN, wxString( wxT("地图下移\tDown") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuFile->Append( m_menuItemDOWN );
	
	wxMenuItem* m_menuItemLeft;
	m_menuItemLeft = new wxMenuItem( m_MenuFile, ID_MAPLEFT, wxString( wxT("地图左移\tLeft") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuFile->Append( m_menuItemLeft );
	
	wxMenuItem* m_menuItemRIGHT;
	m_menuItemRIGHT = new wxMenuItem( m_MenuFile, ID_MAPRIGHT, wxString( wxT("地图右移\tRight") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuFile->Append( m_menuItemRIGHT );
	
	m_menubar3->Append( m_MenuFile, wxT("地图(&MAP)") ); 
	
	m_menuEdit = new wxMenu();
	wxMenuItem* m_menuItemUndo;
	m_menuItemUndo = new wxMenuItem( m_menuEdit, wxID_UNDO, wxString( wxT("撤销") ) + wxT('\t') + wxT("CTRL+Z"), wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItemUndo );
	m_menuItemUndo->Enable( false );
	
	wxMenuItem* m_menuItemRedo;
	m_menuItemRedo = new wxMenuItem( m_menuEdit, wxID_REDO, wxString( wxT("重做") ) + wxT('\t') + wxT("CTRL+Y"), wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItemRedo );
	m_menuItemRedo->Enable( false );
	
	m_menubar3->Append( m_menuEdit, wxT("编辑(&Edit)") ); 
	
	m_MenuCharacter = new wxMenu();
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_MenuCharacter, wxID_ANY, wxString( wxT("加载人物") ) + wxT('\t') + wxT("CTRL+R"), wxT("加载一个人物等待放置"), wxITEM_NORMAL );
	m_MenuCharacter->Append( m_menuItem5 );
	
	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( m_MenuCharacter, ID_DIRECTION, wxString( wxT("下一方向") ) + wxT('\t') + wxT("Space"), wxT("人物的方向(Dir)"), wxITEM_NORMAL );
	m_MenuCharacter->Append( m_menuItem6 );
	
	m_MenuCharacter->AppendSeparator();
	
	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( m_MenuCharacter, wxID_ANY, wxString( wxT("导入NPC文件") ) + wxT('\t') + wxT("CTRL+I"), wxEmptyString, wxITEM_NORMAL );
	m_MenuCharacter->Append( m_menuItem8 );
	
	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( m_MenuCharacter, wxID_ANY, wxString( wxT("导出NPC文件") ) + wxT('\t') + wxT("CTRL+N"), wxEmptyString, wxITEM_NORMAL );
	m_MenuCharacter->Append( m_menuItem7 );
	
	m_MenuCharacter->AppendSeparator();
	
	wxMenuItem* m_menuItemShowUnShowNpc;
	m_menuItemShowUnShowNpc = new wxMenuItem( m_MenuCharacter, wxID_ANY, wxString( wxT("修复人物位置...") ) + wxT('\t') + wxT("CTRL+H"), wxEmptyString, wxITEM_NORMAL );
	m_MenuCharacter->Append( m_menuItemShowUnShowNpc );
	
	m_MenuCharacter->AppendSeparator();
	
	wxMenuItem* m_menuItem9;
	m_menuItem9 = new wxMenuItem( m_MenuCharacter, wxID_ANY, wxString( wxT("人物数量") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuCharacter->Append( m_menuItem9 );
	
	wxMenuItem* m_menuItem10;
	m_menuItem10 = new wxMenuItem( m_MenuCharacter, wxID_ANY, wxString( wxT("清空人物") ) , wxT("清空地图上所有NPC"), wxITEM_NORMAL );
	m_MenuCharacter->Append( m_menuItem10 );
	
	m_menubar3->Append( m_MenuCharacter, wxT("人物( &NPC )") ); 
	
	m_menu3 = new wxMenu();
	wxMenuItem* m_menuItem11;
	m_menuItem11 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("加载物品") ) + wxT('\t') + wxT("CTRL+W"), wxT("加载一个物品等待放置"), wxITEM_NORMAL );
	m_menu3->Append( m_menuItem11 );
	
	wxMenuItem* m_menuItem12;
	m_menuItem12 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("下一方向") ) + wxT('\t') + wxT("Space"), wxT("物品的方向(Dir)"), wxITEM_NORMAL );
	m_menu3->Append( m_menuItem12 );
	
	m_menu3->AppendSeparator();
	
	wxMenuItem* m_menuItem13;
	m_menuItem13 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("导入OBJ文件") ) + wxT('\t') + wxT("CTRL+L"), wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem13 );
	
	wxMenuItem* m_menuItem14;
	m_menuItem14 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("导出OBJ文件") ) + wxT('\t') + wxT("CTRL+B"), wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem14 );
	
	m_menu3->AppendSeparator();
	
	wxMenuItem* m_menuItemShowUnShowOBj;
	m_menuItemShowUnShowOBj = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("修复物品位置...") ) + wxT('\t') + wxT("CTRL+J"), wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItemShowUnShowOBj );
	
	m_menu3->AppendSeparator();
	
	wxMenuItem* m_menuItem15;
	m_menuItem15 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("物品数量") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem15 );
	
	wxMenuItem* m_menuItem16;
	m_menuItem16 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("清空物品") ) , wxT("清空地图上所有OBJ"), wxITEM_NORMAL );
	m_menu3->Append( m_menuItem16 );
	
	m_menubar3->Append( m_menu3, wxT("物品(&OBJ)") ); 
	
	m_menu5 = new wxMenu();
	wxMenuItem* m_menuItem25;
	m_menuItem25 = new wxMenuItem( m_menu5, wxID_ANY, wxString( wxT("帧率(&FPS)...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( m_menuItem25 );
	
	m_menubar3->Append( m_menu5, wxT("性能(&P)") ); 
	
	m_menu6 = new wxMenu();
	wxMenuItem* m_menuItem27;
	m_menuItem27 = new wxMenuItem( m_menu6, wxID_ANY, wxString( wxT("端口(&Port)") ) , wxT("当连接TMX地图读取服务失败时，可尝试设置其它端口"), wxITEM_NORMAL );
	m_menu6->Append( m_menuItem27 );
	
	m_menubar3->Append( m_menu6, wxT("TMX(&T)") ); 
	
	this->SetMenuBar( m_menubar3 );
	
	m_menuMapView = new wxMenu();
	wxMenuItem* m_menuItemCopy;
	m_menuItemCopy = new wxMenuItem( m_menuMapView, MYID_MAPVIEW_COPY, wxString( wxT("复制") ) , wxT("设置为放置源"), wxITEM_NORMAL );
	m_menuMapView->Append( m_menuItemCopy );
	
	wxMenuItem* m_menuItemCut;
	m_menuItemCut = new wxMenuItem( m_menuMapView, MYID_MAPVIEW_CUT, wxString( wxT("剪切") ) , wxT("设置为放置源并删除"), wxITEM_NORMAL );
	m_menuMapView->Append( m_menuItemCut );
	
	wxMenuItem* m_menuItemPaste;
	m_menuItemPaste = new wxMenuItem( m_menuMapView, MYID_MAPVIEW_PASTE, wxString( wxT("粘贴") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuMapView->Append( m_menuItemPaste );
	
	m_menuMapView->AppendSeparator();
	
	wxMenuItem* m_menuItemDetail;
	m_menuItemDetail = new wxMenuItem( m_menuMapView, MYID_MAPVIEW_DETIAL, wxString( wxT("详细...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuMapView->Append( m_menuItemDetail );
	
	wxMenuItem* m_menuItemDelete;
	m_menuItemDelete = new wxMenuItem( m_menuMapView, MYID_MAPVIEW_DELETE, wxString( wxT("删除") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuMapView->Append( m_menuItemDelete );
	
	m_menuMapView->AppendSeparator();
	
	wxMenuItem* m_menuItemBatEdit;
	m_menuItemBatEdit = new wxMenuItem( m_menuMapView, MYID_MAPVIEW_BATEDIT, wxString( wxT("批量编辑...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuMapView->Append( m_menuItemBatEdit );
	
	wxMenuItem* m_menuItem26;
	m_menuItem26 = new wxMenuItem( m_menuMapView, MYID_MAPVIEW_CLEAR_SELECTION, wxString( wxT("消除选择") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuMapView->Append( m_menuItem26 );
	
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( MapFrameBase::MapFrameBaseOnContextMenu ), NULL, this ); 
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MapFrameBase::OnClose ) );
	this->Connect( m_tool7->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OpenMap ) );
	m_Layer1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer1 ), NULL, this );
	m_Layer2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer2 ), NULL, this );
	m_Layer3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer3 ), NULL, this );
	m_Trap->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnTrap ), NULL, this );
	m_Barrer->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnBarrer ), NULL, this );
	m_LayerTransparent->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayerTransparent ), NULL, this );
	this->Connect( m_tool5->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnNpcMode ) );
	this->Connect( m_tool6->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnObjMode ) );
	this->Connect( m_ToolPlace->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnPlaceMode ) );
	this->Connect( m_ToolDelete->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnDeleteMode ) );
	this->Connect( m_ToolEditAttribute->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnEditAttributeMode ) );
	this->Connect( m_tool4->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnMoveMode ) );
	this->Connect( m_tool8->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnShowNpcCheck ) );
	this->Connect( m_tool9->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnShowObjCheck ) );
	m_MapView->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MapFrameBase::OnMapViewMouseLeftDown ), NULL, this );
	m_MapView->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( MapFrameBase::OnMapViewMouseLeftUp ), NULL, this );
	m_MapView->Connect( wxEVT_MOTION, wxMouseEventHandler( MapFrameBase::OnMouseMove ), NULL, this );
	m_MapView->Connect( wxEVT_PAINT, wxPaintEventHandler( MapFrameBase::OnMapDraw ), NULL, this );
	m_MapView->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( MapFrameBase::OnMapViewMouseRightUp ), NULL, this );
	m_MapControl->Connect( wxEVT_MOTION, wxMouseEventHandler( MapFrameBase::OnMapCtrlMouseMotion ), NULL, this );
	m_MapControl->Connect( wxEVT_PAINT, wxPaintEventHandler( MapFrameBase::OnDrawMapControl ), NULL, this );
	m_npcListCtrl->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( MapFrameBase::OnListCtrlLeftDClick ), NULL, this );
	m_npcListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_FOCUSED, wxListEventHandler( MapFrameBase::OnListItemFocused ), NULL, this );
	m_npcListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( MapFrameBase::OnListCtrlRightDown ), NULL, this );
	m_objListCtrl->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( MapFrameBase::OnListCtrlLeftDClick ), NULL, this );
	m_objListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_FOCUSED, wxListEventHandler( MapFrameBase::OnListItemFocused ), NULL, this );
	m_objListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( MapFrameBase::OnListCtrlRightDown ), NULL, this );
	this->Connect( m_menuItemOpenMap->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OpenMap ) );
	this->Connect( m_menuItemSavePng->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::SaveToPNG ) );
	this->Connect( m_menuItemUP->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapUp ) );
	this->Connect( m_menuItemDOWN->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapDown ) );
	this->Connect( m_menuItemLeft->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapLeft ) );
	this->Connect( m_menuItemRIGHT->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapRight ) );
	this->Connect( m_menuItemUndo->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnUndo ) );
	this->Connect( m_menuItemRedo->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnRedo ) );
	this->Connect( m_menuItem5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnLoadCharater ) );
	this->Connect( m_menuItem6->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnCharacterDirection ) );
	this->Connect( m_menuItem8->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnImportNpcFile ) );
	this->Connect( m_menuItem7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnOutputNpcFile ) );
	this->Connect( m_menuItemShowUnShowNpc->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnRepositionUnseenNpc ) );
	this->Connect( m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnShowNpcCounts ) );
	this->Connect( m_menuItem10->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnClearNpc ) );
	this->Connect( m_menuItem11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnLoadObject ) );
	this->Connect( m_menuItem12->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnObjectDirection ) );
	this->Connect( m_menuItem13->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnImportObjFile ) );
	this->Connect( m_menuItem14->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnOutputObjFile ) );
	this->Connect( m_menuItemShowUnShowOBj->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnRepositionUnseenObj ) );
	this->Connect( m_menuItem15->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnShowObjCount ) );
	this->Connect( m_menuItem16->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnClearObj ) );
	this->Connect( m_menuItem25->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnSetFps ) );
	this->Connect( m_menuItem27->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnSetTmxHelperPort ) );
	this->Connect( m_menuItemCopy->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	this->Connect( m_menuItemCut->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	this->Connect( m_menuItemPaste->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	this->Connect( m_menuItemDetail->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	this->Connect( m_menuItemDelete->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	this->Connect( m_menuItemBatEdit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	this->Connect( m_menuItem26->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
}

MapFrameBase::~MapFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MapFrameBase::OnClose ) );
	this->Disconnect( m_tool7->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OpenMap ) );
	m_Layer1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer1 ), NULL, this );
	m_Layer2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer2 ), NULL, this );
	m_Layer3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer3 ), NULL, this );
	m_Trap->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnTrap ), NULL, this );
	m_Barrer->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnBarrer ), NULL, this );
	m_LayerTransparent->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayerTransparent ), NULL, this );
	this->Disconnect( m_tool5->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnNpcMode ) );
	this->Disconnect( m_tool6->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnObjMode ) );
	this->Disconnect( m_ToolPlace->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnPlaceMode ) );
	this->Disconnect( m_ToolDelete->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnDeleteMode ) );
	this->Disconnect( m_ToolEditAttribute->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnEditAttributeMode ) );
	this->Disconnect( m_tool4->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnMoveMode ) );
	this->Disconnect( m_tool8->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnShowNpcCheck ) );
	this->Disconnect( m_tool9->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MapFrameBase::OnShowObjCheck ) );
	m_MapView->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MapFrameBase::OnMapViewMouseLeftDown ), NULL, this );
	m_MapView->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( MapFrameBase::OnMapViewMouseLeftUp ), NULL, this );
	m_MapView->Disconnect( wxEVT_MOTION, wxMouseEventHandler( MapFrameBase::OnMouseMove ), NULL, this );
	m_MapView->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MapFrameBase::OnMapDraw ), NULL, this );
	m_MapView->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( MapFrameBase::OnMapViewMouseRightUp ), NULL, this );
	m_MapControl->Disconnect( wxEVT_MOTION, wxMouseEventHandler( MapFrameBase::OnMapCtrlMouseMotion ), NULL, this );
	m_MapControl->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MapFrameBase::OnDrawMapControl ), NULL, this );
	m_npcListCtrl->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( MapFrameBase::OnListCtrlLeftDClick ), NULL, this );
	m_npcListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_FOCUSED, wxListEventHandler( MapFrameBase::OnListItemFocused ), NULL, this );
	m_npcListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( MapFrameBase::OnListCtrlRightDown ), NULL, this );
	m_objListCtrl->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( MapFrameBase::OnListCtrlLeftDClick ), NULL, this );
	m_objListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_FOCUSED, wxListEventHandler( MapFrameBase::OnListItemFocused ), NULL, this );
	m_objListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( MapFrameBase::OnListCtrlRightDown ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OpenMap ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::SaveToPNG ) );
	this->Disconnect( ID_MAPUP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapUp ) );
	this->Disconnect( ID_MAPDOWN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapDown ) );
	this->Disconnect( ID_MAPLEFT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapLeft ) );
	this->Disconnect( ID_MAPRIGHT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapRight ) );
	this->Disconnect( wxID_UNDO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnUndo ) );
	this->Disconnect( wxID_REDO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnRedo ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnLoadCharater ) );
	this->Disconnect( ID_DIRECTION, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnCharacterDirection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnImportNpcFile ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnOutputNpcFile ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnRepositionUnseenNpc ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnShowNpcCounts ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnClearNpc ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnLoadObject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnObjectDirection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnImportObjFile ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnOutputObjFile ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnRepositionUnseenObj ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnShowObjCount ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnClearObj ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnSetFps ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnSetTmxHelperPort ) );
	this->Disconnect( MYID_MAPVIEW_COPY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	this->Disconnect( MYID_MAPVIEW_CUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	this->Disconnect( MYID_MAPVIEW_PASTE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	this->Disconnect( MYID_MAPVIEW_DETIAL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	this->Disconnect( MYID_MAPVIEW_DELETE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	this->Disconnect( MYID_MAPVIEW_BATEDIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	this->Disconnect( MYID_MAPVIEW_CLEAR_SELECTION, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapViewMenu ) );
	
	delete m_menuMapView; 
}

NpcItemEditDialogBase::NpcItemEditDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerTop;
	bSizerTop = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizerTop222;
	bSizerTop222 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText54 = new wxStaticText( this, wxID_ANY, wxT("提示：双击属性名称打开相应文件"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText54->Wrap( -1 );
	bSizer29->Add( m_staticText54, 0, wxALL, 5 );
	
	m_GridManager = new wxPropertyGridManager(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPGMAN_DEFAULT_STYLE|wxPG_DESCRIPTION|wxPG_TOOLBAR);
	m_GridManager->SetExtraStyle( wxPG_EX_HIDE_PAGE_BUTTONS|wxPG_EX_MODE_BUTTONS ); 
	
	m_PropertyPage = m_GridManager->AddPage( wxT("Page"), wxNullBitmap );
	bSizer29->Add( m_GridManager, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizerTop222->Add( bSizer29, 1, wxEXPAND, 5 );
	
	
	bSizerTop->Add( bSizerTop222, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_button26 = new wxButton( this, wxID_ANY, wxT("保存..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_button26->SetToolTip( wxT("存为INI文件") );
	
	bSizer11->Add( m_button26, 0, wxALL, 5 );
	
	
	bSizer11->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_OK = new wxButton( this, wxID_ANY, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_OK, 0, wxALL, 5 );
	
	m_Cancle = new wxButton( this, wxID_ANY, wxT("取消"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_Cancle, 0, wxALL, 5 );
	
	
	bSizerTop->Add( bSizer11, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerTop );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( NpcItemEditDialogBase::OnClose ) );
	m_button26->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NpcItemEditDialogBase::OnSaveNpcIniFile ), NULL, this );
	m_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NpcItemEditDialogBase::OnOk ), NULL, this );
	m_Cancle->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NpcItemEditDialogBase::OnCancle ), NULL, this );
}

NpcItemEditDialogBase::~NpcItemEditDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( NpcItemEditDialogBase::OnClose ) );
	m_button26->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NpcItemEditDialogBase::OnSaveNpcIniFile ), NULL, this );
	m_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NpcItemEditDialogBase::OnOk ), NULL, this );
	m_Cancle->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( NpcItemEditDialogBase::OnCancle ), NULL, this );
	
}

ObjItemEditDialogBase::ObjItemEditDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	m_GridManager = new wxPropertyGridManager(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPGMAN_DEFAULT_STYLE|wxPG_DESCRIPTION|wxPG_TOOLBAR);
	m_GridManager->SetExtraStyle( wxPG_EX_HIDE_PAGE_BUTTONS|wxPG_EX_MODE_BUTTONS ); 
	
	m_propertyGridPage2 = m_GridManager->AddPage( wxT("Page"), wxNullBitmap );
	bSizer13->Add( m_GridManager, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer12->Add( bSizer13, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_button25 = new wxButton( this, wxID_ANY, wxT("保存..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_button25->SetToolTip( wxT("存为INI文件") );
	
	bSizer16->Add( m_button25, 0, wxALL, 5 );
	
	
	bSizer16->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_OK = new wxButton( this, wxID_ANY, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_OK, 0, wxALL, 5 );
	
	m_Cancel = new wxButton( this, wxID_ANY, wxT("取消"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_Cancel, 0, wxALL, 5 );
	
	
	bSizer12->Add( bSizer16, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer12 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ObjItemEditDialogBase::OnClose ) );
	m_button25->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjItemEditDialogBase::OnSaveObjIniFile ), NULL, this );
	m_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjItemEditDialogBase::OnOk ), NULL, this );
	m_Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjItemEditDialogBase::OnCancle ), NULL, this );
}

ObjItemEditDialogBase::~ObjItemEditDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ObjItemEditDialogBase::OnClose ) );
	m_button25->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjItemEditDialogBase::OnSaveObjIniFile ), NULL, this );
	m_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjItemEditDialogBase::OnOk ), NULL, this );
	m_Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjItemEditDialogBase::OnCancle ), NULL, this );
	
}

SetTmxHelperPortDialogBase::SetTmxHelperPortDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer27->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText53 = new wxStaticText( this, wxID_ANY, wxT("端口："), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53->Wrap( -1 );
	bSizer27->Add( m_staticText53, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_spinCtrlPort = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 1 );
	bSizer27->Add( m_spinCtrlPort, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer27->Add( 0, 0, 2, wxEXPAND, 5 );
	
	
	bSizer25->Add( bSizer27, 0, wxEXPAND, 5 );
	
	
	bSizer24->Add( bSizer25, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer26->Add( 0, 0, 3, wxEXPAND, 5 );
	
	m_buttonOK = new wxButton( this, wxID_ANY, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( m_buttonOK, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer26->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bSizer24->Add( bSizer26, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer24 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SetTmxHelperPortDialogBase::OnButtonOKClick ), NULL, this );
}

SetTmxHelperPortDialogBase::~SetTmxHelperPortDialogBase()
{
	// Disconnect Events
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SetTmxHelperPortDialogBase::OnButtonOKClick ), NULL, this );
	
}
