#ifndef ICONLOADER_H
#define ICONLOADER_H


class IconLoader : public Node {
 GDCLASS(IconLoader, Node);

 public:

 ImageTexture load_icon_from_name(const String &icon_name);

 IconLoader();
 ~IconLoader();

 protected:
 static void _bind_methods();

 //tool
};


#endif
