
#include "thirdparty/clipper2/include/clipper2/clipper.h"

Vector<Vector<Point2>> Geometry2D::_polypaths_do_operation(PolyBooleanOperation p_op, const Vector<Point2> &p_polypath_a, const Vector<Point2> &p_polypath_b, bool is_a_open) {
	Vector<Vector<Vector2>> finished_polygons;

#ifdef CLIPPER_ENABLED
	using namespace Clipper2Lib;

	Paths64 polyon_paths_solution;

	ClipType clipper_cliptype = ClipType::Union;
	FillRule clipper_fillrule = FillRule::EvenOdd;

	if (is_a_open) {
		// Polyline with Polygon
		Clipper64 clipper_64;

		Paths64 polyline_paths;
		Paths64 polygon_paths_b;

		Path64 polyline_path_a;
		Path64 polygon_path_b;

		for (const Vector2 &polyline_point : p_polypath_a) {
			const Point64 &point = Point64(polyline_point.x * (real_t)SCALE_FACTOR, polyline_point.y * (real_t)SCALE_FACTOR);
			polyline_path_a.push_back(point);
		}
		polyline_paths.push_back(polyline_path_a);

		for (const Vector2 &polypath_outline_point : p_polypath_b) {
			const Point64 &point = Point64(polypath_outline_point.x * (real_t)SCALE_FACTOR, polypath_outline_point.y * (real_t)SCALE_FACTOR);
			polygon_path_b.push_back(point);
		}
		polygon_paths_b.push_back(polygon_path_b);

		switch (p_op) {
			case OPERATION_UNION:
				// not supported for polyline (in Godot)
				return finished_polygons;

			case OPERATION_DIFFERENCE:
				clipper_cliptype = ClipType::Difference;
				clipper_fillrule = FillRule::EvenOdd;
				clipper_64.AddOpenSubject(polyline_paths);
				clipper_64.AddClip(polygon_paths_b);
				break;

			case OPERATION_INTERSECTION:
				clipper_cliptype = ClipType::Intersection;
				clipper_fillrule = FillRule::EvenOdd;
				clipper_64.AddOpenSubject(polyline_paths);
				clipper_64.AddClip(polygon_paths_b);
				break;

			case OPERATION_XOR:
				// not supported for polyline
				return finished_polygons;
		}

		Paths64 polygon_solution, polyline_solution;
		clipper_64.Execute(clipper_cliptype, clipper_fillrule, polygon_solution, polyline_solution);
		polyon_paths_solution = polyline_solution;

	} else {
		// Polygon with Polygon
		Paths64 polygon_paths;
		Paths64 polygon_clip_paths;

		Path64 polygon_path_a;
		Path64 polygon_path_b;

		for (const Vector2 &polypath_outline_point : p_polypath_a) {
			const Point64 &point = Point64(polypath_outline_point.x * (real_t)SCALE_FACTOR, polypath_outline_point.y * (real_t)SCALE_FACTOR);
			polygon_path_a.push_back(point);
		}
		polygon_paths.push_back(polygon_path_a);

		for (const Vector2 &polypath_outline_point : p_polypath_b) {
			const Point64 &point = Point64(polypath_outline_point.x * (real_t)SCALE_FACTOR, polypath_outline_point.y * (real_t)SCALE_FACTOR);
			polygon_path_b.push_back(point);
		}
		polygon_clip_paths.push_back(polygon_path_b);

		switch (p_op) {
			case OPERATION_UNION:
				clipper_cliptype = ClipType::Union;
				clipper_fillrule = FillRule::NonZero;

				polyon_paths_solution = Union(polygon_paths, polygon_clip_paths, clipper_fillrule);
				break;
			case OPERATION_DIFFERENCE:
				clipper_cliptype = ClipType::Difference;
				clipper_fillrule = FillRule::EvenOdd;

				polyon_paths_solution = Difference(polygon_paths, polygon_clip_paths, clipper_fillrule);
				break;
			case OPERATION_INTERSECTION:
				clipper_cliptype = ClipType::Intersection;
				clipper_fillrule = FillRule::NonZero;

				polyon_paths_solution = Intersect(polygon_paths, polygon_clip_paths, clipper_fillrule);
				break;
			case OPERATION_XOR:
				clipper_cliptype = ClipType::Xor;
				clipper_fillrule = FillRule::NonZero;

				polyon_paths_solution = Xor(polygon_paths, polygon_clip_paths, clipper_fillrule);
				break;
		}
	}

	for (const Path64 &polyon_path : polyon_paths_solution) {
		Vector<Vector2> finished_polygon;
		for (const Point64 &polyon_path_point : polyon_path) {
			finished_polygon.push_back(Vector2(static_cast<real_t>(polyon_path_point.x), static_cast<real_t>(polyon_path_point.y)) / (real_t)SCALE_FACTOR);
		}
		finished_polygons.push_back(finished_polygon);
	}
#endif // CLIPPER_ENABLED

	return finished_polygons;
}

Vector<Vector<Point2>> Geometry2D::_polypath_offset(const Vector<Point2> &p_polypath, real_t p_delta, PolyJoinType p_join_type, PolyEndType p_end_type) {
	Vector<Vector<Vector2>> finished_polygons;

#ifdef CLIPPER_ENABLED
	using namespace Clipper2Lib;

	JoinType clipper_jointype = JoinType::Miter;

	switch (p_join_type) {
		case JOIN_SQUARE:
			clipper_jointype = JoinType::Square;
			break;
		case JOIN_ROUND:
			clipper_jointype = JoinType::Round;
			break;
		case JOIN_MITER:
			clipper_jointype = JoinType::Miter;
			break;
	}

	EndType clipper_endtype = EndType::Polygon;

	switch (p_end_type) {
		case END_POLYGON:
			clipper_endtype = EndType::Polygon;
			break;
		case END_JOINED:
			clipper_endtype = EndType::Joined;
			break;
		case END_BUTT:
			clipper_endtype = EndType::Butt;
			break;
		case END_SQUARE:
			clipper_endtype = EndType::Square;
			break;
		case END_ROUND:
			clipper_endtype = EndType::Round;
			break;
	}

	Paths64 polygon_paths;

	Path64 polygon_path;
	for (const Vector2 &polypath_outline_point : p_polypath) {
		const Point64 &point = Point64(polypath_outline_point.x * (real_t)SCALE_FACTOR, polypath_outline_point.y * (real_t)SCALE_FACTOR);
		polygon_path.push_back(point);
	}
	polygon_paths.push_back(polygon_path);

	Paths64 paths_solution = InflatePaths(polygon_paths, p_delta, clipper_jointype, clipper_endtype);

	for (const Path64 &scaled_path : paths_solution) {
		Vector<Vector2> polypath;
		for (const Point64 &scaled_point : scaled_path) {
			polypath.push_back(Vector2(static_cast<real_t>(scaled_point.x), static_cast<real_t>(scaled_point.y)) / (real_t)SCALE_FACTOR);
		}
		finished_polygons.push_back(polypath);
	}
#endif // CLIPPER_ENABLED

	return finished_polygons;
}

