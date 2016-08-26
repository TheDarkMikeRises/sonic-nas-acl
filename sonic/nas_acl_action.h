
/*
 * Copyright (c) 2016 Dell Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 * FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 * See the Apache Version 2.0 License for specific language governing
 * permissions and limitations under the License.
 */


/*!
 * \file   nas_acl_action.h
 * \brief  NAS ACL Action entry Class Definition
 * \date   04-2015
 */

#ifndef _NAS_ACL_ACTION_H_
#define _NAS_ACL_ACTION_H_

#include "dell-base-acl.h"
#include "std_assert.h"
#include "nas_base_utils.h"
#include "nas_ndi_acl.h"
#include "nas_acl_common.h"
#include <string.h>
#include <vector>
#include <unordered_map>

using ndi_acl_action_list_t = std::vector<ndi_acl_entry_action_t>;

class nas_acl_action_t
{
    public:
        static bool is_type_valid (BASE_ACL_ACTION_TYPE_t type) noexcept {
            return nas_acl_action_is_type_valid (type);
        }

        static const char* type_name (BASE_ACL_ACTION_TYPE_t type) noexcept {
            return nas_acl_action_type_name (type);
        }

        bool is_counter () const noexcept
        { return (action_type() == BASE_ACL_ACTION_TYPE_SET_COUNTER); }

        const char* name () const noexcept;

        void dbg_dump () const;

        nas_acl_action_t (BASE_ACL_ACTION_TYPE_t t);

        BASE_ACL_ACTION_TYPE_t action_type () const noexcept {return _a_info.action_type;}
        void set_values_type (ndi_acl_action_values_type_t type) {_a_info.values_type = type;};

        const nas::ifindex_list_t& get_action_if_list () const noexcept;

        void set_u8_action_val (const nas_acl_common_data_list_t& data_list);
        void set_u16_action_val (const nas_acl_common_data_list_t& data_list);
        void set_u32_action_val (const nas_acl_common_data_list_t& data_list);
        void set_obj_id_action_val (const nas_acl_common_data_list_t& data_list);
        void set_ipv4_action_val (const nas_acl_common_data_list_t& data_list);
        void set_ipv6_action_val (const nas_acl_common_data_list_t& data_list);
        void set_mac_action_val (const nas_acl_common_data_list_t& val_list);
        void set_opaque_data_action_val (const nas_acl_common_data_list_t& data_list);
        void set_opaque_data_list_action_val (const nas_acl_common_data_list_t& data_list);
        void set_log_action_val (const nas_acl_common_data_list_t& data_list);
        void set_action_ifindex (const nas_acl_common_data_list_t& data_list);
        void set_action_ifindex_list (const nas_acl_common_data_list_t& data_list);
        void set_pkt_action_val (const nas_acl_common_data_list_t& data_list);
        void set_ndi_counter_ids (const nas::ndi_obj_id_table_t & ndi_obj_id_table);

        void get_u8_action_val (nas_acl_common_data_list_t& data_list) const;
        void get_u16_action_val (nas_acl_common_data_list_t& data_list) const;
        void get_u32_action_val (nas_acl_common_data_list_t& data_list) const;
        void get_obj_id_action_val (nas_acl_common_data_list_t& data_list) const;
        void get_ipv4_action_val (nas_acl_common_data_list_t& data_list) const;
        void get_ipv6_action_val (nas_acl_common_data_list_t& data_list) const;
        void get_mac_action_val (nas_acl_common_data_list_t& val_list) const;
        void get_opaque_data_action_val (nas_acl_common_data_list_t& data_list) const;
        void get_log_action_val (nas_acl_common_data_list_t& data_list) const;
        void get_action_ifindex (nas_acl_common_data_list_t& data_list) const;
        void get_action_ifindex_list (nas_acl_common_data_list_t& data_list) const;
        void get_pkt_action_val (nas_acl_common_data_list_t& val_list) const;

        nas_obj_id_t  counter_id () const noexcept {return _nas_oid;}

        bool copy_action_ndi (ndi_acl_action_list_t& ndi_alist,
                              npu_id_t npu_id, nas::mem_alloc_helper_t& m) const;

        bool operator!= (const nas_acl_action_t& second) const;

    private:
        void _set_opaque_data (const nas_acl_common_data_list_t& data_list);
        bool _ndi_copy_one_obj_id (ndi_acl_entry_action_t& ndi_action,
                                   npu_id_t npu_id) const;
        bool _ndi_copy_obj_id_list (ndi_acl_entry_action_t& ndi_action,
                                    npu_id_t npu_id,
                                    nas::mem_alloc_helper_t& mem_trakr) const;

        // Value for In/Out port/port-list Action and
        // Value for Redirect_port action
        nas::ifindex_list_t      _ifindex_list;

        // Value for Counter Action
        nas_obj_id_t             _nas_oid = 0;

        // Values for following Actions are stored as table of
        // nas-obj-id <-> ndi_obj_id_table mapping:
        //   - REDIRECT_PORT - ONLY if the port is a lag -
        //                     ifindex is used as nas_obj_id
        //   - Mirror, IP Nexthop, CPU Queue Action.
        //          Basically any action that takes Opaque data as param.
        std::unordered_map<nas_obj_id_t, nas::ndi_obj_id_table_t>  _nas2ndi_oid_tbl;

        // Values for all other actions are stored directly in NDI structure
        ndi_acl_entry_action_t   _a_info;
};

inline const nas::ifindex_list_t&
nas_acl_action_t::get_action_if_list () const noexcept
{
    return _ifindex_list;
}

inline const char* nas_acl_action_t::name () const noexcept
{
    return type_name (action_type());
}

#endif
