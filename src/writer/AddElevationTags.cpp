// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <string>
#include <osmium/memory/buffer.hpp>
#include <osmium/osm/node.hpp>
#include "util/index/ElevationIndex.h"
#include "global/Constants.h"
#include "writer/AddElevationTags.h"

using writer::AddElevationTags;
using global::INVALID_ELEV;
using global::DEFAULT_ELE_TAG;
using util::index::ElevationIndex;

// ____________________________________________________________________________
AddElevationTags::AddElevationTags(
    osmium::memory::Buffer& buffer,
    ElevationIndex* elevationIndex,
    const std::string& elevationTag,
    const bool overwrite) :
    m_buffer(buffer),
    _elevationIndex(elevationIndex),
    _elevationTag(elevationTag),
    _overwrite(overwrite) {}

// ____________________________________________________________________________
void AddElevationTags::node(const osmium::Node& node) {
  {
    osmium::builder::NodeBuilder builder{m_buffer};
    // Copy common object attributes over to the new node.
    copy_attributes(builder, node);

    // Copy the location over to the new node.
    builder.set_location(node.location());

    const std::string elevation =
      std::to_string(_elevationIndex->getElevation(node.id()));

    if (!_overwrite) {
      buildTagsAddElevation(node, builder, elevation);
    } else {
      buildTagsUpdateElevation(node, builder, elevation);
    }
  }
  m_buffer.commit();
}

// ____________________________________________________________________________
void AddElevationTags::buildTagsAddElevation(
    const osmium::Node& node,
    osmium::builder::NodeBuilder& builder,
    const std::string& elevation) {
  // Copy the existing tags and check if the default
  // elevation tag ("ele") was present.
  bool defaultTagPresent = false;
  osmium::builder::TagListBuilder tagBuilder{builder};
  for (const auto& tag : node.tags()) {
    tagBuilder.add_tag(tag);
    if (!std::strcmp(tag.key(), DEFAULT_ELE_TAG)) {
      defaultTagPresent = true;
    }
  }
  // Add the elevation tag if the elevation is valid and the
  // default elevation tag was not present before.
  if (elevation != std::to_string(INVALID_ELEV) && !defaultTagPresent) {
    tagBuilder.add_tag(_elevationTag, elevation);
  }
}

// ____________________________________________________________________________
void AddElevationTags::buildTagsUpdateElevation(
    const osmium::Node& node,
    osmium::builder::NodeBuilder& builder,
    const std::string& elevation) {
  // Copy the existing tags excluding the elevation tag.
  bool elevationTagPresent = false;
  osmium::builder::TagListBuilder tagBuilder{builder};
  for (const auto& tag : node.tags()) {
    if (std::strcmp(tag.key(), _elevationTag.c_str())) {
      tagBuilder.add_tag(tag);
    } else {
      elevationTagPresent = true;
    }
  }
  // Add the elevation tag with the updated elevation if valid.
  if (elevation != std::to_string(INVALID_ELEV)) {
    tagBuilder.add_tag(_elevationTag, elevation);
  } else if (elevationTagPresent) {
    tagBuilder.add_tag(_elevationTag, node.tags()[_elevationTag.c_str()]);
  }
}

// ____________________________________________________________________________
void AddElevationTags::way(const osmium::Way& way) {
  {
    osmium::builder::WayBuilder builder{m_buffer};
    copy_attributes(builder, way);
    builder.add_item(way.tags());

    // Copy the node list over to the new way.
    builder.add_item(way.nodes());
  }
  m_buffer.commit();
}

// ____________________________________________________________________________
void AddElevationTags::relation(const osmium::Relation& relation) {
  {
    osmium::builder::RelationBuilder builder{m_buffer};
    copy_attributes(builder, relation);
    builder.add_item(relation.tags());

    // Copy the relation member list over to the new way.
    builder.add_item(relation.members());
  }
  m_buffer.commit();
}
