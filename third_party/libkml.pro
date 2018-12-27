include("../config.pri")
include("libkml.pri")
include("port.pri")


QT -= gui

TARGET = libkml
TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

#git https://github.com/libkml/libkml.git

message($$_PRO_FILE_PWD_)

DEFINES += unix

android{
    INCLUDEPATH += libkml-1.2.0/third_party/expat.win32
    }

INCLUDEPATH += libkml-1.2.0/src/km

DESTDIR = $${PROJECT_LIBS}

#unix{
#    DESTDIR = ${PROJECT_LIBS}
#}

android{
#    API_VERSION = android-21
#    DESTDIR = ${PROJECT_LIBS}
    #IF NOT build, please check that Build Environent Set ANDROID_NDK_PLATFORM to android-21 !! (default android-9 does not compile)
}

QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter


SOURCES += \
    libkml-1.2.0/src/kml/base/attributes.cc \
    libkml-1.2.0/src/kml/base/csv_splitter.cc \
    libkml-1.2.0/src/kml/base/date_time.cc \
    libkml-1.2.0/src/kml/base/expat_handler_ns.cc \
    libkml-1.2.0/src/kml/base/expat_parser.cc \
    libkml-1.2.0/src/kml/base/file.cc \
    libkml-1.2.0/src/kml/base/file_posix.cc \
    libkml-1.2.0/src/kml/base/math_util.cc \
    libkml-1.2.0/src/kml/base/mimetypes.cc \
    libkml-1.2.0/src/kml/base/referent.cc \
    libkml-1.2.0/src/kml/base/string_util.cc \
    libkml-1.2.0/src/kml/base/time_util.cc \
    libkml-1.2.0/src/kml/base/uri_parser.cc \
    libkml-1.2.0/src/kml/base/version.cc \
    libkml-1.2.0/src/kml/base/xml_namespaces.cc \
    libkml-1.2.0/src/kml/base/zip_file.cc \
    libkml-1.2.0/src/kml/convenience/atom_util.cc \
    libkml-1.2.0/src/kml/convenience/convenience.cc \
    libkml-1.2.0/src/kml/convenience/csv_file.cc \
    libkml-1.2.0/src/kml/convenience/csv_parser.cc \
    libkml-1.2.0/src/kml/convenience/feature_list.cc \
    libkml-1.2.0/src/kml/convenience/google_doc_list.cc \
    libkml-1.2.0/src/kml/convenience/google_maps_data.cc \
    libkml-1.2.0/src/kml/convenience/google_picasa_web.cc \
    libkml-1.2.0/src/kml/convenience/google_spreadsheets.cc \
    libkml-1.2.0/src/kml/convenience/http_client.cc \
    libkml-1.2.0/src/kml/convenience/kmz_check_links.cc \
    libkml-1.2.0/src/kml/dom/abstractlatlonbox.cc \
    libkml-1.2.0/src/kml/dom/abstractview.cc \
    libkml-1.2.0/src/kml/dom/atom.cc \
    libkml-1.2.0/src/kml/dom/balloonstyle.cc \
    libkml-1.2.0/src/kml/dom/colorstyle.cc \
    libkml-1.2.0/src/kml/dom/container.cc \
    libkml-1.2.0/src/kml/dom/document.cc \
    libkml-1.2.0/src/kml/dom/element.cc \
    libkml-1.2.0/src/kml/dom/extendeddata.cc \
    libkml-1.2.0/src/kml/dom/feature.cc \
    libkml-1.2.0/src/kml/dom/folder.cc \
    libkml-1.2.0/src/kml/dom/geometry.cc \
    libkml-1.2.0/src/kml/dom/gx_timeprimitive.cc \
    libkml-1.2.0/src/kml/dom/gx_tour.cc \
    libkml-1.2.0/src/kml/dom/hotspot.cc \
    libkml-1.2.0/src/kml/dom/iconstyle.cc \
    libkml-1.2.0/src/kml/dom/kml.cc \
    libkml-1.2.0/src/kml/dom/kml22.cc \
    libkml-1.2.0/src/kml/dom/kml_cast.cc \
    libkml-1.2.0/src/kml/dom/kml_factory.cc \
    libkml-1.2.0/src/kml/dom/kml_handler.cc \
    libkml-1.2.0/src/kml/dom/kml_handler_ns.cc \
    libkml-1.2.0/src/kml/dom/labelstyle.cc \
    libkml-1.2.0/src/kml/dom/linestyle.cc \
    libkml-1.2.0/src/kml/dom/link.cc \
    libkml-1.2.0/src/kml/dom/liststyle.cc \
    libkml-1.2.0/src/kml/dom/model.cc \
    libkml-1.2.0/src/kml/dom/networklink.cc \
    libkml-1.2.0/src/kml/dom/networklinkcontrol.cc \
    libkml-1.2.0/src/kml/dom/object.cc \
    libkml-1.2.0/src/kml/dom/overlay.cc \
    libkml-1.2.0/src/kml/dom/parser.cc \
    libkml-1.2.0/src/kml/dom/placemark.cc \
    libkml-1.2.0/src/kml/dom/polystyle.cc \
    libkml-1.2.0/src/kml/dom/region.cc \
    libkml-1.2.0/src/kml/dom/schema.cc \
    libkml-1.2.0/src/kml/dom/serializer.cc \
    libkml-1.2.0/src/kml/dom/snippet.cc \
    libkml-1.2.0/src/kml/dom/style.cc \
    libkml-1.2.0/src/kml/dom/stylemap.cc \
    libkml-1.2.0/src/kml/dom/styleselector.cc \
    libkml-1.2.0/src/kml/dom/substyle.cc \
    libkml-1.2.0/src/kml/dom/timeprimitive.cc \
    libkml-1.2.0/src/kml/dom/vec2.cc \
    libkml-1.2.0/src/kml/dom/visitor.cc \
    libkml-1.2.0/src/kml/dom/visitor_driver.cc \
    libkml-1.2.0/src/kml/dom/xal.cc \
    libkml-1.2.0/src/kml/dom/xml_serializer.cc \
    libkml-1.2.0/src/kml/dom/xsd.cc \
    libkml-1.2.0/src/kml/engine/clone.cc \
    libkml-1.2.0/src/kml/engine/entity_mapper.cc \
    libkml-1.2.0/src/kml/engine/feature_balloon.cc \
    libkml-1.2.0/src/kml/engine/feature_view.cc \
    libkml-1.2.0/src/kml/engine/feature_visitor.cc \
    libkml-1.2.0/src/kml/engine/find.cc \
    libkml-1.2.0/src/kml/engine/find_xml_namespaces.cc \
    libkml-1.2.0/src/kml/engine/get_link_parents.cc \
    libkml-1.2.0/src/kml/engine/get_links.cc \
    libkml-1.2.0/src/kml/engine/href.cc \
    libkml-1.2.0/src/kml/engine/id_mapper.cc \
    libkml-1.2.0/src/kml/engine/kml_cache.cc \
    libkml-1.2.0/src/kml/engine/kml_file.cc \
    libkml-1.2.0/src/kml/engine/kml_stream.cc \
    libkml-1.2.0/src/kml/engine/kml_uri.cc \
    libkml-1.2.0/src/kml/engine/kmz_cache.cc \
    libkml-1.2.0/src/kml/engine/kmz_file.cc \
    libkml-1.2.0/src/kml/engine/link_util.cc \
    libkml-1.2.0/src/kml/engine/location_util.cc \
    libkml-1.2.0/src/kml/engine/merge.cc \
    libkml-1.2.0/src/kml/engine/parse_old_schema.cc \
    libkml-1.2.0/src/kml/engine/style_inliner.cc \
    libkml-1.2.0/src/kml/engine/style_merger.cc \
    libkml-1.2.0/src/kml/engine/style_resolver.cc \
    libkml-1.2.0/src/kml/engine/style_splitter.cc \
    libkml-1.2.0/src/kml/engine/update.cc \
    libkml-1.2.0/src/kml/engine/update_processor.cc \
    libkml-1.2.0/src/kml/regionator/feature_list_region_handler.cc \
    libkml-1.2.0/src/kml/regionator/regionator.cc \
    libkml-1.2.0/src/kml/regionator/regionator_util.cc \
    libkml-1.2.0/src/kml/xsd/xsd_complex_type.cc \
    libkml-1.2.0/src/kml/xsd/xsd_element.cc \
    libkml-1.2.0/src/kml/xsd/xsd_file.cc \
    libkml-1.2.0/src/kml/xsd/xsd_handler.cc \
    libkml-1.2.0/src/kml/xsd/xsd_primitive_type.cc \
    libkml-1.2.0/src/kml/xsd/xsd_util.cc \
    libkml-1.2.0/src/kml/xsd/xst_parser.cc \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriCommon.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriCompare.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriEscape.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriFile.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriIp4.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriIp4Base.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriNormalize.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriNormalizeBase.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriParse.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriParseBase.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriQuery.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriRecompose.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriResolve.c \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriShorten.c \
    libkml-1.2.0/third_party/zlib-1.2.3/contrib/minizip/ioapi.c \
    libkml-1.2.0/third_party/zlib-1.2.3/contrib/minizip/iomem_simple.c \
    libkml-1.2.0/third_party/zlib-1.2.3/contrib/minizip/miniunz.c \
    libkml-1.2.0/third_party/zlib-1.2.3/contrib/minizip/minizip.c \
    libkml-1.2.0/third_party/zlib-1.2.3/contrib/minizip/mztools.c \
    libkml-1.2.0/third_party/zlib-1.2.3/contrib/minizip/unzip.c \
    libkml-1.2.0/third_party/zlib-1.2.3/contrib/minizip/zip.c

HEADERS += \
    libkml-1.2.0/src/kml/base/attributes.h \
    libkml-1.2.0/src/kml/base/color32.h \
    libkml-1.2.0/src/kml/base/csv_splitter.h \
    libkml-1.2.0/src/kml/base/date_time.h \
    libkml-1.2.0/src/kml/base/expat_handler.h \
    libkml-1.2.0/src/kml/base/expat_handler_ns.h \
    libkml-1.2.0/src/kml/base/expat_parser.h \
    libkml-1.2.0/src/kml/base/file.h \
    libkml-1.2.0/src/kml/base/math_util.h \
    libkml-1.2.0/src/kml/base/memory_file.h \
    libkml-1.2.0/src/kml/base/mimetypes.h \
    libkml-1.2.0/src/kml/base/net_cache.h \
    libkml-1.2.0/src/kml/base/referent.h \
    libkml-1.2.0/src/kml/base/string_util.h \
    libkml-1.2.0/src/kml/base/tempfile.h \
    libkml-1.2.0/src/kml/base/time_util.h \
    libkml-1.2.0/src/kml/base/uri_parser.h \
    libkml-1.2.0/src/kml/base/util.h \
    libkml-1.2.0/src/kml/base/vec3.h \
    libkml-1.2.0/src/kml/base/version.h \
    libkml-1.2.0/src/kml/base/xml_element.h \
    libkml-1.2.0/src/kml/base/xml_file.h \
    libkml-1.2.0/src/kml/base/xml_namespaces.h \
    libkml-1.2.0/src/kml/base/xmlns.h \
    libkml-1.2.0/src/kml/base/zip_file.h \
    libkml-1.2.0/src/kml/convenience/atom_util.h \
    libkml-1.2.0/src/kml/convenience/convenience.h \
    libkml-1.2.0/src/kml/convenience/csv_file.h \
    libkml-1.2.0/src/kml/convenience/csv_parser.h \
    libkml-1.2.0/src/kml/convenience/element_counter.h \
    libkml-1.2.0/src/kml/convenience/feature_list.h \
    libkml-1.2.0/src/kml/convenience/google_doc_list.h \
    libkml-1.2.0/src/kml/convenience/google_maps_data.h \
    libkml-1.2.0/src/kml/convenience/google_picasa_web.h \
    libkml-1.2.0/src/kml/convenience/google_spreadsheets.h \
    libkml-1.2.0/src/kml/convenience/gpx_trk_pt_handler.h \
    libkml-1.2.0/src/kml/convenience/http_client.h \
    libkml-1.2.0/src/kml/convenience/http_client_test_util.h \
    libkml-1.2.0/src/kml/convenience/kmz_check_links.h \
    libkml-1.2.0/src/kml/dom/abstractlatlonbox.h \
    libkml-1.2.0/src/kml/dom/abstractview.h \
    libkml-1.2.0/src/kml/dom/atom.h \
    libkml-1.2.0/src/kml/dom/balloonstyle.h \
    libkml-1.2.0/src/kml/dom/colorstyle.h \
    libkml-1.2.0/src/kml/dom/container.h \
    libkml-1.2.0/src/kml/dom/document.h \
    libkml-1.2.0/src/kml/dom/element.h \
    libkml-1.2.0/src/kml/dom/extendeddata.h \
    libkml-1.2.0/src/kml/dom/feature.h \
    libkml-1.2.0/src/kml/dom/folder.h \
    libkml-1.2.0/src/kml/dom/geometry.h \
    libkml-1.2.0/src/kml/dom/gx_timeprimitive.h \
    libkml-1.2.0/src/kml/dom/gx_tour.h \
    libkml-1.2.0/src/kml/dom/hotspot.h \
    libkml-1.2.0/src/kml/dom/iconstyle.h \
    libkml-1.2.0/src/kml/dom/kml.h \
    libkml-1.2.0/src/kml/dom/kml22.h \
    libkml-1.2.0/src/kml/dom/kml_cast.h \
    libkml-1.2.0/src/kml/dom/kml_factory.h \
    libkml-1.2.0/src/kml/dom/kml_funcs.h \
    libkml-1.2.0/src/kml/dom/kml_handler.h \
    libkml-1.2.0/src/kml/dom/kml_handler_ns.h \
    libkml-1.2.0/src/kml/dom/kml_ptr.h \
    libkml-1.2.0/src/kml/dom/kmldom.h \
    libkml-1.2.0/src/kml/dom/labelstyle.h \
    libkml-1.2.0/src/kml/dom/linestyle.h \
    libkml-1.2.0/src/kml/dom/link.h \
    libkml-1.2.0/src/kml/dom/liststyle.h \
    libkml-1.2.0/src/kml/dom/model.h \
    libkml-1.2.0/src/kml/dom/networklink.h \
    libkml-1.2.0/src/kml/dom/networklinkcontrol.h \
    libkml-1.2.0/src/kml/dom/object.h \
    libkml-1.2.0/src/kml/dom/overlay.h \
    libkml-1.2.0/src/kml/dom/parser.h \
    libkml-1.2.0/src/kml/dom/parser_observer.h \
    libkml-1.2.0/src/kml/dom/placemark.h \
    libkml-1.2.0/src/kml/dom/polystyle.h \
    libkml-1.2.0/src/kml/dom/region.h \
    libkml-1.2.0/src/kml/dom/schema.h \
    libkml-1.2.0/src/kml/dom/serializer.h \
    libkml-1.2.0/src/kml/dom/snippet.h \
    libkml-1.2.0/src/kml/dom/stats_serializer.h \
    libkml-1.2.0/src/kml/dom/style.h \
    libkml-1.2.0/src/kml/dom/stylemap.h \
    libkml-1.2.0/src/kml/dom/styleselector.h \
    libkml-1.2.0/src/kml/dom/substyle.h \
    libkml-1.2.0/src/kml/dom/timeprimitive.h \
    libkml-1.2.0/src/kml/dom/vec2.h \
    libkml-1.2.0/src/kml/dom/visitor.h \
    libkml-1.2.0/src/kml/dom/visitor_driver.h \
    libkml-1.2.0/src/kml/dom/xal.h \
    libkml-1.2.0/src/kml/dom/xml_serializer.h \
    libkml-1.2.0/src/kml/dom/xsd.h \
    libkml-1.2.0/src/kml/engine/bbox.h \
    libkml-1.2.0/src/kml/engine/clone.h \
    libkml-1.2.0/src/kml/engine/engine_constants.h \
    libkml-1.2.0/src/kml/engine/engine_types.h \
    libkml-1.2.0/src/kml/engine/entity_mapper.h \
    libkml-1.2.0/src/kml/engine/feature_balloon.h \
    libkml-1.2.0/src/kml/engine/feature_view.h \
    libkml-1.2.0/src/kml/engine/feature_visitor.h \
    libkml-1.2.0/src/kml/engine/find.h \
    libkml-1.2.0/src/kml/engine/find_xml_namespaces.h \
    libkml-1.2.0/src/kml/engine/get_link_parents.h \
    libkml-1.2.0/src/kml/engine/get_links.h \
    libkml-1.2.0/src/kml/engine/href.h \
    libkml-1.2.0/src/kml/engine/id_mapper.h \
    libkml-1.2.0/src/kml/engine/id_mapper_internal.h \
    libkml-1.2.0/src/kml/engine/kml_cache.h \
    libkml-1.2.0/src/kml/engine/kml_file.h \
    libkml-1.2.0/src/kml/engine/kml_stream.h \
    libkml-1.2.0/src/kml/engine/kml_uri.h \
    libkml-1.2.0/src/kml/engine/kml_uri_internal.h \
    libkml-1.2.0/src/kml/engine/kmz_cache.h \
    libkml-1.2.0/src/kml/engine/kmz_file.h \
    libkml-1.2.0/src/kml/engine/link_util.h \
    libkml-1.2.0/src/kml/engine/location_util.h \
    libkml-1.2.0/src/kml/engine/merge.h \
    libkml-1.2.0/src/kml/engine/object_id_parser_observer.h \
    libkml-1.2.0/src/kml/engine/old_schema_parser_observer.h \
    libkml-1.2.0/src/kml/engine/parse_old_schema.h \
    libkml-1.2.0/src/kml/engine/schema_parser_observer.h \
    libkml-1.2.0/src/kml/engine/shared_style_parser_observer.h \
    libkml-1.2.0/src/kml/engine/style_inliner.h \
    libkml-1.2.0/src/kml/engine/style_inliner_internal.h \
    libkml-1.2.0/src/kml/engine/style_merger.h \
    libkml-1.2.0/src/kml/engine/style_resolver.h \
    libkml-1.2.0/src/kml/engine/style_splitter.h \
    libkml-1.2.0/src/kml/engine/style_splitter_internal.h \
    libkml-1.2.0/src/kml/engine/update.h \
    libkml-1.2.0/src/kml/engine/update_processor.h \
    libkml-1.2.0/src/kml/regionator/feature_list_region_handler.h \
    libkml-1.2.0/src/kml/regionator/feature_list_regionator.h \
    libkml-1.2.0/src/kml/regionator/region_handler.h \
    libkml-1.2.0/src/kml/regionator/regionator.h \
    libkml-1.2.0/src/kml/regionator/regionator_qid.h \
    libkml-1.2.0/src/kml/regionator/regionator_util.h \
    libkml-1.2.0/src/kml/xsd/xsd_complex_type.h \
    libkml-1.2.0/src/kml/xsd/xsd_element.h \
    libkml-1.2.0/src/kml/xsd/xsd_file.h \
    libkml-1.2.0/src/kml/xsd/xsd_handler.h \
    libkml-1.2.0/src/kml/xsd/xsd_primitive_type.h \
    libkml-1.2.0/src/kml/xsd/xsd_schema.h \
    libkml-1.2.0/src/kml/xsd/xsd_simple_type.h \
    libkml-1.2.0/src/kml/xsd/xsd_type.h \
    libkml-1.2.0/src/kml/xsd/xsd_util.h \
    libkml-1.2.0/src/kml/xsd/xst_parser.h \
    libkml-1.2.0/src/kml/dom.h \
    libkml-1.2.0/src/kml/engine.h \
    libkml-1.2.0/src/stdafx.h \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/abi/borland_prefix.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/abi/borland_suffix.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/abi/msvc_prefix.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/abi/msvc_suffix.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/borland.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/comeau.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/common_edg.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/compaq_cxx.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/digitalmars.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/gcc.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/gcc_xml.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/greenhills.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/hp_acc.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/intel.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/kai.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/metrowerks.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/mpw.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/sgi_mipspro.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/sunpro_cc.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/vacpp.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/compiler/visualc.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/no_tr1/complex.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/no_tr1/functional.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/no_tr1/memory.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/no_tr1/utility.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/platform/aix.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/platform/amigaos.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/platform/beos.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/platform/bsd.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/platform/cygwin.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/platform/hpux.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/platform/irix.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/platform/linux.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/platform/macos.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/platform/qnxnto.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/platform/solaris.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/platform/win32.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/stdlib/dinkumware.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/stdlib/libcomo.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/stdlib/libstdcpp3.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/stdlib/modena.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/stdlib/msl.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/stdlib/roguewave.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/stdlib/sgi.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/stdlib/stlport.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/stdlib/vacpp.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/abi_prefix.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/abi_suffix.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/auto_link.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/posix_features.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/requires_threads.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/select_compiler_config.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/select_platform_config.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/select_stdlib_config.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/suffix.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config/user.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/detail/workaround.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/assert.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/checked_delete.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/config.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/intrusive_ptr.hpp \
    libkml-1.2.0/third_party/boost_1_34_1/boost/scoped_ptr.hpp \
    libkml-1.2.0/third_party/expat.win32/expat.h \
    libkml-1.2.0/third_party/expat.win32/expat_external.h \
    libkml-1.2.0/third_party/uriparser-0.7.5/include/uriparser/Uri.h \
    libkml-1.2.0/third_party/uriparser-0.7.5/include/uriparser/UriBase.h \
    libkml-1.2.0/third_party/uriparser-0.7.5/include/uriparser/UriDefsAnsi.h \
    libkml-1.2.0/third_party/uriparser-0.7.5/include/uriparser/UriDefsConfig.h \
    libkml-1.2.0/third_party/uriparser-0.7.5/include/uriparser/UriDefsUnicode.h \
    libkml-1.2.0/third_party/uriparser-0.7.5/include/uriparser/UriIp4.h \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriCommon.h \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriIp4Base.h \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriNormalizeBase.h \
    libkml-1.2.0/third_party/uriparser-0.7.5/lib/UriParseBase.h \
    libkml-1.2.0/third_party/zlib-1.2.3/contrib/minizip/crypt.h \
    libkml-1.2.0/third_party/zlib-1.2.3/contrib/minizip/ioapi.h \
    libkml-1.2.0/third_party/zlib-1.2.3/contrib/minizip/mztools.h \
    libkml-1.2.0/third_party/zlib-1.2.3/contrib/minizip/unzip.h \
    libkml-1.2.0/third_party/zlib-1.2.3/contrib/minizip/zip.h \
    libkml-1.2.0/third_party/zlib-1.2.3/zconf.h \
    libkml-1.2.0/third_party/zlib-1.2.3/zlib.h \
    libkml-1.2.0/xcode/LibKML/LibKML.h


