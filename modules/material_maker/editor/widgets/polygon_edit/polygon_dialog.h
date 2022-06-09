#ifndef POLYGON_DIALOG_H
#define POLYGON_DIALOG_H


class PolygonDialog : public WindowDialog {
 GDCLASS(PolygonDialog, WindowDialog);

 public:

 bool get_closed() const;
 void set_closed(const bool val);

 PoolVector2Array get_previous_points();
 void set_previous_points(const PoolVector2Array &val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 void set_closed(const bool c = true);
 void _on_CurveDialog_popup_hide();
 void _on_OK_pressed();
 void _on_Cancel_pressed();
 void edit_polygon(const Variant &poly);
 void _on_PolygonEditor_value_changed(const Variant &value);

 PolygonDialog();
 ~PolygonDialog();

 protected:
 static void _bind_methods();

 //tool
 //export 
 // setget set_closed
 bool closed = true;
 PoolVector2Array previous_points = ;
 Variant ;
 signal polygon_changed(polygon);
 //var result = yield(self, "return_polygon")
 //queue_free()
 //return result
};


#endif
