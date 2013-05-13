/***************************************************************************************************
 *  Copyright 2013 maidsafe.net limited                                                            *
 *                                                                                                 *
 *  The following source code is property of MaidSafe.net limited and is not meant for external    *
 *  use. The use of this code is governed by the licence file licence.txt found in the root of     *
 *  this directory and also on www.maidsafe.net.                                                   *
 *                                                                                                 *
 *  You are not free to copy, amend or otherwise use this source code without the explicit written *
 *  permission of the board of directors of MaidSafe.net.                                          *
 **************************************************************************************************/

#include "maidsafe/data_types/world_directory.h"

#include "maidsafe/common/types.h"
#include "maidsafe/common/crypto.h"
#include "maidsafe/common/error.h"

#include "maidsafe/data_types/directory_types.pb.h"


namespace maidsafe {

const DataTagValue WorldDirectoryTag::kEnumValue = DataTagValue::kWorldDirectoryValue;

WorldDirectory::WorldDirectory(const WorldDirectory& other)
    : name_(other.name_),
      data_(other.data_),
      signature_(other.signature_) {}

WorldDirectory& WorldDirectory::operator=(const WorldDirectory& other) {
  name_ = other.name_;
  data_ = other.data_;
  signature_ = other.signature_;
  return *this;
}

WorldDirectory::WorldDirectory(WorldDirectory&& other)
    : name_(std::move(other.name_)),
      data_(std::move(other.data_)),
      signature_(std::move(other.signature_)) {}

WorldDirectory& WorldDirectory::operator=(WorldDirectory&& other) {
  name_ = std::move(other.name_);
  data_ = std::move(other.data_);
  signature_ = std::move(other.signature_);
  return *this;
}


WorldDirectory::WorldDirectory(const name_type& name, const NonEmptyString& data)
    : name_(name),
      data_(data),
      signature_() {}

WorldDirectory::WorldDirectory(const name_type& name,
                         const NonEmptyString& data,
                         const asymm::PrivateKey& signing_key)
    : name_(name),
      data_(data),
      signature_(asymm::Sign(data, signing_key)) {}

WorldDirectory::WorldDirectory(const name_type& name,
                               const serialised_type& serialised_mutable_data)
    : name_(name),
      data_(),
      signature_() {
  protobuf::WorldDirectory proto_mutable_data;
  if (!proto_mutable_data.ParseFromString(serialised_mutable_data.data.string()))
    ThrowError(CommonErrors::parsing_error);
  data_ = NonEmptyString(proto_mutable_data.data());
  if (proto_mutable_data.has_signature())
    signature_ = asymm::Signature(proto_mutable_data.signature());
}

WorldDirectory::serialised_type WorldDirectory::Serialise() const {
  protobuf::WorldDirectory proto_mutable_data;
  proto_mutable_data.set_data(data_.string());
  if (signature_.IsInitialised())
    proto_mutable_data.set_signature(signature_.string());
  return serialised_type(NonEmptyString(proto_mutable_data.SerializeAsString()));
}

}  // namespace maidsafe
