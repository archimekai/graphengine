/**
 * Copyright 2019-2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef INC_FRAMEWORK_COMMON_GE_TYPES_H_
#define INC_FRAMEWORK_COMMON_GE_TYPES_H_

#include <stdint.h>

#include <string>
#include <vector>

#include "common/fmk_error_codes.h"
#include "ge/ge_api_error_codes.h"

using std::string;

namespace ge {
enum RuntimeType { HOST = 0, DEVICE = 1 };

enum PerfLevel { GEN_TASK_WITH_FUSION = -1, GEN_TASK_WITHOUT_L2FUSION = 3, GEN_TASK_WITHOUT_FUSION = 4 };

enum FrameworkType {
  FMK_TYPE_C = 0,
  FMK_TYPE_MINDSPORE = 1,
  FMK_TYPE_T = 3,
  FMK_TYPE_A_NN,
  FMK_TYPE_RESERVED,
};

const char *const GE_ENGINE_ATTR_MEM_TYPE_HBM = "HBM";

// Data cache, including data address and length
struct DataBuffer {
 public:
  void *data;       // Data address
  uint32_t length;  // Data length
  bool isDataSupportMemShare = false;
  DataBuffer(void *dataIn, uint32_t len, bool isSupportMemShare)
      : data(dataIn), length(len), isDataSupportMemShare(isSupportMemShare) {}

  DataBuffer() : data(nullptr), length(0), isDataSupportMemShare(false) {}
};

///
/// @ingroup domi_ome
/// @brief External inputdata
///
struct InputData {
  uint32_t index;                 // Index of input data
  uint32_t timestamp;             // Data creation time
  uint32_t timeout;               // Processing timeout
  uint32_t model_id;              // Model ID required for data processing
  uint64_t request_id = 0;        // Request ID
  std::vector<DataBuffer> blobs;  // Actual input data, currently only supports one input
};

// The definition of output result structure
struct OutputData {
  uint32_t index;     // Index of input data
  uint32_t model_id;  // The model ID corresponding to the processing result

  /// Output data cache, arranged in sequence of output operators.
  /// If the operator has multiple outputs,
  /// the data buffer order of the operator is the same as that defined in the
  /// offline model
  std::vector<DataBuffer> blobs;
};

// The definition of command data structure
struct Command {
  std::string cmd_type;                 // Command type
  std::vector<std::string> cmd_params;  // Command params
};

// The definition of I/O shape description
struct ShapeDescription {
  int64_t num = 0;
  int64_t channel = 0;
  int64_t height = 0;
  int64_t width = 0;
  std::vector<int64_t> dims;
};

// Definition of input and output description information
struct InputOutputDescInfo {
  std::string name;
  uint32_t size;
  uint32_t data_type;
  ShapeDescription shape_info;
};

// The structure of offline Modeldata
struct ModelData {
  void *model_data = nullptr;  // Model binary data start addr
  uint32_t model_len = 0;      // Model binary data length
  int32_t priority = 0;        // Model priority
  std::string key;             // Key path for encrypt model, Empty for unencrypt
};

// The definition of Model information
struct ModelInfo {
  uint32_t version = 0;
  std::string name;
  bool is_encrypt = 0;  //  0:unencrypt, 1:encrypt
  std::vector<ShapeDescription> input_desc;
  std::vector<ShapeDescription> output_desc;
  uint8_t reserved[3] = {0};  // 3-byte reserved field
};

// Asynchronous callback interface, implemented by the caller
class ModelListener {
 public:
  virtual ~ModelListener() {}
  ///
  /// @brief Asynchronous callback interface
  /// @param [in] model_id   Model ID of the callback
  /// @param [in] data_index Index of the input_data
  /// @param [in] resultCode Execution results
  ///
  virtual Status OnComputeDone(uint32_t model_id, uint32_t data_index, uint32_t result_code) = 0;
};

// OMM configuration item
struct Options {
  int64_t session_id;
  int32_t device_id;
  int64_t job_id;
  bool isUseHcom;
  bool deployMode;
  bool isAICPUMode;
  bool enable_atomic;
  string podName;
  int64_t rankId;
  string rankTableFile;
  int32_t ge_hccl_flag = 0;
  int32_t physical_device_id;
};
}  // namespace ge

#endif  // INC_FRAMEWORK_COMMON_GE_TYPES_H_
