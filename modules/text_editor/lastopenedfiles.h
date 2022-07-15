#ifndef LASTOPENEDFILES_H
#define LASTOPENEDFILES_H

//This will not need EditorPlugin and EditorSettings, as now it can get them using singletons

class LastOpenedFiles : public Reference {
	GDCLASS(LastOpenedFiles, Reference);

public:
	EditorPlugin get_ *editor_plugin();
	void set_ *editor_plugin(const EditorPlugin &val);

	Ref<EditorSettings> get_editor_settings();
	void set_editor_settings(const Ref<EditorSettings> &val);

	void store_opened_files(const Control &filecontainer);
	void remove_opened_file(const int index, const Control &filecontainer);
	Array load_opened_files();
	void store_editor_fonts(const String &file_name, const String &font_path);
	String get_editor_font();

	LastOpenedFiles();
	~LastOpenedFiles();

protected:
	static void _bind_methods();

	//tool
	EditorPlugin *editor_plugin = null;
	Ref<EditorSettings> editor_settings = null;
};

#endif
