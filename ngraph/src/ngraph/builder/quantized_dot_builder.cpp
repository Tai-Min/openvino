//*****************************************************************************
// Copyright 2017-2020 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*****************************************************************************

#include <memory>

#include "ngraph/builder/quantized_dot_builder.hpp"

using namespace std;
using namespace ngraph;

namespace ngraph
{
    namespace builder
    {
        shared_ptr<Node> QuantizedDotBuilder(const Output<Node>& input0,
                                             const Output<Node>& input1,
                                             const size_t reduction_axes_count,
                                             const Output<Node>& min_input0,
                                             const Output<Node>& max_input0,
                                             const Output<Node>& min_input1,
                                             const Output<Node>& max_input1,
                                             const Output<Node>& min_output,
                                             const Output<Node>& max_output,
                                             const ngraph::element::Type& output_type,
                                             const ngraph::AxisSet& input0_axes,
                                             const ngraph::AxisSet& input1_axes,
                                             const ngraph::AxisSet& output_axes)
        {
            auto input0_scale =
                quantization_utils::get_scale(min_input0, max_input0, input0.get_element_type());
            auto input1_scale =
                quantization_utils::get_scale(min_input1, max_input1, input1.get_element_type());
            auto output_scale = quantization_utils::get_scale(min_output, max_output, output_type);

            // For Builders the zero point is assumed to be zero (for now)
            auto input0_zero_point = op::Constant::create(input0.get_element_type(), Shape{}, {0});
            auto input1_zero_point = op::Constant::create(input1.get_element_type(), Shape{}, {0});
            auto output_zero_point = op::Constant::create(output_type, Shape{}, {0});

            return make_shared<op::QuantizedDot>(input0,
                                                 input1,
                                                 reduction_axes_count,
                                                 input0_scale,
                                                 input0_zero_point,
                                                 input1_scale,
                                                 input1_zero_point,
                                                 output_scale,
                                                 output_zero_point,
                                                 output_type,
                                                 input0_axes,
                                                 input1_axes,
                                                 output_axes)
                ->add_provenance_group_members_above({input0, input1});
        }
    }
}
