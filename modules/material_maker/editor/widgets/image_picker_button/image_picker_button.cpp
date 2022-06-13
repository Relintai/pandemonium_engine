
#include "image_picker_button.h"

#include "../file_dialog/mat_maker_file_dialog.h"
#include "core/io/resource_loader.h"
#include "scene/resources/texture.h"

String ImagePickerButton::get_image_path() {
	return image_path;
}

void ImagePickerButton::set_image_path(const String &path) {
	do_set_image_path(path);
	emit_signal("on_file_selected", path);
}

void ImagePickerButton::do_set_image_path(const String &path) {
	if (path == "") {
		return;
	}

	image_path = path;

	Ref<Image> img = ResourceLoader::load("image_path", "Image");
	Ref<ImageTexture> tex = get_normal_texture();

	if (!tex.is_valid()) {
		tex.instance();
	}

	tex->create_from_image(img);

	set_normal_texture(tex);
}

void ImagePickerButton::_on_ImagePicker_pressed() {
	if (dialog) {
		dialog->queue_delete();
		dialog = nullptr;
	}

	dialog = memnew(MatMakerFileDialog);
	add_child(dialog);
	dialog->set_custom_minimum_size(Vector2(500, 500));
	dialog->set_access(FileDialog::ACCESS_FILESYSTEM);
	dialog->set_mode(FileDialog::MODE_OPEN_FILE);
	dialog->add_filter("*.bmp;BMP Image");
	dialog->add_filter("*.exr;EXR Image");
	dialog->add_filter("*.hdr;Radiance HDR Image");
	dialog->add_filter("*.jpg,*.jpeg;JPEG Image");
	dialog->add_filter("*.png;PNG Image");
	dialog->add_filter("*.svg;SVG Image");
	dialog->add_filter("*.tga;TGA Image");
	dialog->add_filter("*.webp;WebP Image");
	dialog->connect("return_paths", this, "on_file_selected");
	dialog->select_files();
}

void ImagePickerButton::on_file_selected(const Array &files) {
	if (files.size() > 0) {
		set_image_path(files[0]);
	}

	if (dialog) {
		dialog->queue_delete();
		dialog = nullptr;
	}
}

void ImagePickerButton::on_drop_image_file(const String &file_name) {
	set_image_path(file_name);
}

ImagePickerButton::ImagePickerButton() {
	dialog = nullptr;

	set_custom_minimum_size(Vector2(64, 64));
	set_clip_contents(true);

	Ref<ImageTexture> imagepicker_prop_texture_normal;
	imagepicker_prop_texture_normal.instance();
	set_normal_texture(imagepicker_prop_texture_normal);
	set_expand(true);
	set_stretch_mode(STRETCH_KEEP_ASPECT_CENTERED);
}

ImagePickerButton::~ImagePickerButton() {
}

void ImagePickerButton::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		Ref<ImageTexture> it;
		it.instance();

		set_normal_texture(it);

		connect("pressed", this, "_on_ImagePicker_pressed");
	}
}

void ImagePickerButton::_bind_methods() {
	ADD_SIGNAL(MethodInfo("on_file_selected", PropertyInfo(Variant::STRING, "file")));

	ClassDB::bind_method(D_METHOD("get_image_path"), &ImagePickerButton::get_image_path);
	ClassDB::bind_method(D_METHOD("set_image_path", "path"), &ImagePickerButton::set_image_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "image_path"), "set_image_path", "get_image_path");

	ClassDB::bind_method(D_METHOD("do_set_image_path", "path"), &ImagePickerButton::do_set_image_path);
	ClassDB::bind_method(D_METHOD("set_image_path", "path"), &ImagePickerButton::set_image_path);
	ClassDB::bind_method(D_METHOD("_on_ImagePicker_pressed"), &ImagePickerButton::_on_ImagePicker_pressed);
	ClassDB::bind_method(D_METHOD("on_drop_image_file", "file_name"), &ImagePickerButton::on_drop_image_file);
	ClassDB::bind_method(D_METHOD("on_file_selected", "files"), &ImagePickerButton::on_file_selected);
}
