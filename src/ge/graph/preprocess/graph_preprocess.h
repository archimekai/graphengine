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

#ifndef GE_GRAPH_PREPROCESS_GRAPH_PREPROCESS_H_
#define GE_GRAPH_PREPROCESS_GRAPH_PREPROCESS_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "common/debug/log.h"
#include "common/debug/memory_dumper.h"
#include "common/model_parser/base.h"
#include "common/properties_manager.h"
#include "common/string_util.h"
#include "common/types.h"
#include "common/util.h"
#include "graph/compute_graph.h"
#include "graph/manager/graph_manager_utils.h"
#include "graph/model.h"
#include "graph/node.h"
#include "graph/utils/graph_utils.h"
#include "graph/utils/tensor_utils.h"
#include "omg/omg_inner_types.h"
#include "runtime/context.h"

namespace ge {
class GraphPrepare {
 public:
  GraphPrepare();
  virtual ~GraphPrepare();
  GraphPrepare(const GraphPrepare &in) = delete;
  GraphPrepare &operator=(const GraphPrepare &in) = delete;
  Status Prepare(ConstGraphPtr graph, const std::vector<GeTensor> &user_input,
                 ge::ComputeGraphPtr &compute_graph, uint64_t session_id = 0);
  void SetOptions(const GraphManagerOptions &options);

 private:
  Status Init(const ge::Graph &graph, uint64_t session_id = 0);
  Status Preprocess(const std::vector<GeTensor> &user_input);
  Status CheckGraph();
  Status SetRtContext(rtContext_t rt_context, rtCtxMode_t mode);
  Status AdjustDataOpOutput(const NodePtr &node);
  Status UpdateInput(const std::vector<GeTensor> &user_input);
  Status CheckConstOp();
  Status VerifyConstOp(const NodePtr &node);
  Status CheckUserInput(const std::vector<GeTensor> &user_input);
  Status OptimizeForPreprocess();
  Status InferShapeForPreprocess();
  Status TryDoAipp();
  Status OptimizeForDataAfterInfershape();
  Status UpdateVariableFormats(ComputeGraphPtr &graph);
  Status FormatAndShapeProcess();
  Status ResourcePairProcess(const std::string& action);
  void ProcessCCEFormat();
  Status OptimizeBeforeInfershape();
  void SaveOriginalGraphToOmModel();
  ge::ComputeGraphPtr compute_graph_;
  GraphManagerOptions options_;
};
}  // namespace ge
#endif  // GE_GRAPH_PREPROCESS_GRAPH_PREPROCESS_H_
