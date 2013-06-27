/* Copyright 2013 MaidSafe.net limited

This MaidSafe Software is licensed under the MaidSafe.net Commercial License, version 1.0 or later,
and The General Public License (GPL), version 3. By contributing code to this project You agree to
the terms laid out in the MaidSafe Contributor Agreement, version 1.0, found in the root directory
of this project at LICENSE, COPYING and CONTRIBUTOR respectively and also available at:

http://www.novinet.com/license

Unless required by applicable law or agreed to in writing, software distributed under the License is
distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied. See the License for the specific language governing permissions and limitations under the
License.
*/

#ifndef MAIDSAFE_DATA_STORE_PERMANENT_STORE_H_
#define MAIDSAFE_DATA_STORE_PERMANENT_STORE_H_

#include <cstdint>
#include <functional>
#include <future>
#include <map>
#include <mutex>
#include <utility>
#include <deque>
#include <set>
#include <vector>

#include "boost/filesystem/path.hpp"
#include "boost/variant.hpp"

#include "maidsafe/common/tagged_value.h"
#include "maidsafe/common/types.h"

#include "maidsafe/data_types/data_name_variant.h"


namespace maidsafe {

namespace data_store {

namespace test { class PermanentStoreTest; }

class PermanentStore {
 public:
  typedef DataNameVariant KeyType;

  PermanentStore(const boost::filesystem::path& disk_path, const DiskUsage& max_disk_usage);
  ~PermanentStore();

  void Put(const KeyType& key, const NonEmptyString& value);
  void Delete(const KeyType& key);
  NonEmptyString Get(const KeyType& key);

  // Return list of elements that should have but not exists yet
  std::vector<KeyType> ElementsToStore(std::set<KeyType> element_list);

  void SetMaxDiskUsage(DiskUsage max_disk_usage);

  DiskUsage GetMaxDiskUsage();
  DiskUsage GetCurrentDiskUsage();

  friend class test::PermanentStoreTest;

 private:
  PermanentStore(const PermanentStore&);
  PermanentStore& operator=(const PermanentStore&);

  boost::filesystem::path GetFilePath(const KeyType& key) const;
  bool HasDiskSpace(const uint64_t& required_space) const;
  boost::filesystem::path KeyToFilePath(const KeyType& key);

  const boost::filesystem::path kDiskPath_;
  DiskUsage max_disk_usage_, current_disk_usage_;
  const uint32_t kDepth_;
  std::mutex mutex_;
  GetIdentityVisitor get_identity_visitor_;
};

}  // namespace data_store

}  // namespace maidsafe


#endif  // MAIDSAFE_DATA_STORE_PERMANENT_STORE_H_
