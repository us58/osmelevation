// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_WRITER_ADDELEVATIONTAGS_H_
#define SRC_WRITER_ADDELEVATIONTAGS_H_

#include <string>
#include <osmium/builder/osm_object_builder.hpp>
#include <osmium/handler.hpp>
#include "util/index/ElevationIndex.h"

namespace writer {

using util::index::ElevationIndex;

// The functions in this class will be called for each object in the input
// and will write a (changed) copy of those objects to the given buffer.
class AddElevationTags : public osmium::handler::Handler {
 public:
  // Constructor. New data will be added to the given buffer.
  explicit AddElevationTags(osmium::memory::Buffer& buffer,
                            ElevationIndex* elevationIndex,
                            const std::string& elevationTag,
                            const bool overwrite);

  // The node handler is called for each node in the input data.
  void node(const osmium::Node& node);

  void buildTagsAddElevation(const osmium::Node& node,
                             osmium::builder::NodeBuilder& builder,
                             const std::string& elevation);

  void buildTagsUpdateElevation(const osmium::Node& node,
                                osmium::builder::NodeBuilder& builder,
                                const std::string& elevation);

  // The way handler is called for each way in the input data.
  void way(const osmium::Way& way);

  // The relation handler is called for each relation in the input data.
  void relation(const osmium::Relation& relation);

 private:
  osmium::memory::Buffer& m_buffer;

  ElevationIndex* _elevationIndex;

  const std::string& _elevationTag;

  const bool _overwrite;

  // Copy attributes common to all OSM objects (nodes, ways, and relations).
  template <typename T>
  void copy_attributes(T& builder, const osmium::OSMObject& object) {
    // The setter functions on the builder object all return the same
    // builder object so they can be chained.
    builder.set_id(object.id())
      .set_version(object.version())
      .set_changeset(object.changeset())
      .set_timestamp(object.timestamp())
      .set_uid(object.uid())
      .set_user(object.user());
  }
};

}  // namespace writer

#endif  // SRC_WRITER_ADDELEVATIONTAGS_H_
