
/*
 * Copyright (c) 2016 Dell Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *  LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 * FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 * See the Apache Version 2.0 License for specific language governing
 * permissions and limitations under the License.
 */


/*!
 * \file   nas_acl_action.cpp
 * \brief  NAS ACL Action entry Class implementation
 */

#include "std_ip_utils.h"
#include "std_mutex_lock.h"
#include "hal_if_mapping.h"
#include "nas_if_utils.h"
#include "nas_vlan_consts.h"
#include "nas_qos_consts.h"
#include "nas_acl_action.h"
#include "nas_acl_log.h"
#include "nas_acl_utl.h"
#include <unordered_map>
#include <arpa/inet.h>
#include <inttypes.h>

static const
std::unordered_map <BASE_ACL_PACKET_ACTION_TYPE_t, const char*, std::hash<int>>
_pkt_action_name_map = {

    {BASE_ACL_PACKET_ACTION_TYPE_DROP,         "DROP"},
    {BASE_ACL_PACKET_ACTION_TYPE_FORWARD,      "FORWARD"},
    {BASE_ACL_PACKET_ACTION_TYPE_COPY_TO_CPU,  "COPY-TO-CPU"},
    {BASE_ACL_PACKET_ACTION_TYPE_TRAP_TO_CPU,  "TRAP-TO-CPU"},
    {BASE_ACL_PACKET_ACTION_TYPE_COPY_TO_CPU_CANCEL,  "COPY-TO-CPU-CANCEL"},
    {BASE_ACL_PACKET_ACTION_TYPE_COPY_TO_CPU_AND_FORWARD,  "COPY-TO-CPU-AND-FORWARD"},
    {BASE_ACL_PACKET_ACTION_TYPE_COPY_TO_CPU_CANCEL_AND_DROP,  "CPU-CANCEL-AND-DROP"},
    {BASE_ACL_PACKET_ACTION_TYPE_COPY_TO_CPU_CANCEL_AND_FORWARD,  "CPU-CANCEL-AND-FORWARD"},
};

nas_acl_action_t::nas_acl_action_t (BASE_ACL_ACTION_TYPE_t t)
{
    if (!is_type_valid (t)) {
        throw nas::base_exception {NAS_ACL_E_ATTR_VAL, __PRETTY_FUNCTION__,
            std::string {"Invalid action type "} + std::to_string (t)};
    }

    memset (&_a_info, 0, sizeof (_a_info));

    _a_info.action_type = t;
    _a_info.values_type = NDI_ACL_ACTION_NO_VALUE;
}

void nas_acl_action_t::set_obj_id_action_val (const nas_acl_common_data_list_t& data_list)
{
    _a_info.values_type = NDI_ACL_ACTION_OBJ_ID;
    _nas_oid = data_list.at(0).obj_id;
}

void nas_acl_action_t::get_obj_id_action_val (nas_acl_common_data_list_t& data_list) const
{
    nas_acl_common_data_t data {};
    data.obj_id = _nas_oid;
    data_list.push_back (data);
}

void nas_acl_action_t::set_u32_action_val (const nas_acl_common_data_list_t& data_list)
{
    _a_info.values_type = NDI_ACL_ACTION_U32;
    _a_info.values.u32  = data_list.at(0).u32;
}

void nas_acl_action_t::get_u32_action_val (nas_acl_common_data_list_t& data_list) const
{
    nas_acl_common_data_t data {};

    data.u32 = _a_info.values.u32;
    data_list.push_back (data);
}

void nas_acl_action_t::set_u16_action_val (const nas_acl_common_data_list_t& data_list)
{
    _a_info.values_type = NDI_ACL_ACTION_U16;
    _a_info.values.u16  = data_list.at(0).u16;
}

void nas_acl_action_t::get_u16_action_val (nas_acl_common_data_list_t& data_list) const
{
    nas_acl_common_data_t data {};

    data.u16 = _a_info.values.u16;
    data_list.push_back (data);
}

void nas_acl_action_t::set_u8_action_val (const nas_acl_common_data_list_t& data_list)
{
    _a_info.values_type = NDI_ACL_ACTION_U8;
    _a_info.values.u8   = data_list.at(0).u8;
}

void nas_acl_action_t::get_u8_action_val (nas_acl_common_data_list_t& data_list) const
{
    nas_acl_common_data_t data {};

    data.u8 = _a_info.values.u8;
    data_list.push_back (data);
}

void nas_acl_action_t::set_ipv4_action_val (const nas_acl_common_data_list_t& data_list)
{
    // IPv4 value can be considered as an array of bytes - copy from the bytes array
    _a_info.values_type = NDI_ACL_ACTION_IPV4_ADDR;
    memcpy ((uint8_t*)&_a_info.values.ipv4, data_list.at(0).bytes.data(),
            sizeof (_a_info.values.ipv4));
}

void nas_acl_action_t::get_ipv4_action_val (nas_acl_common_data_list_t& data_list) const
{
    nas_acl_common_data_t data;

    // IPv4 value can be considered as an array of bytes - copy this into the bytes array
    auto& val = _a_info.values.ipv4;
    auto val_u8 = (uint8_t*) &val;

    data.bytes.insert (data.bytes.begin(), val_u8, val_u8 + sizeof (val));
    data_list.push_back (data);
}

void nas_acl_action_t::set_ipv6_action_val (const nas_acl_common_data_list_t& data_list)
{
    // IPv6 value can be considered as an array of bytes - copy from the bytes array
    _a_info.values_type = NDI_ACL_ACTION_IPV6_ADDR;
    memcpy ((uint8_t*)&_a_info.values.ipv6, data_list.at(0).bytes.data(),
            sizeof (_a_info.values.ipv6));
}

void nas_acl_action_t::get_ipv6_action_val (nas_acl_common_data_list_t& data_list) const
{
    nas_acl_common_data_t data;

    // IPv6 value can be considered as an array of bytes - copy this into the bytes array
    auto& val = _a_info.values.ipv6;
    auto val_u8 = (uint8_t*) (&val);

    data.bytes.insert (data.bytes.begin(), val_u8, val_u8+sizeof (val));
    data_list.push_back (data);
}

void nas_acl_action_t::set_mac_action_val (const nas_acl_common_data_list_t& data_list)
{
    _a_info.values_type = NDI_ACL_ACTION_MAC_ADDR;
    memcpy (_a_info.values.mac, data_list.at(0).bytes.data(), HAL_MAC_ADDR_LEN);
}

void nas_acl_action_t::get_mac_action_val (nas_acl_common_data_list_t& data_list) const
{
    nas_acl_common_data_t data;
    const uint8_t* val = _a_info.values.mac;

    data.bytes.insert (data.bytes.begin(), val, val+HAL_MAC_ADDR_LEN);
    data_list.push_back (data);
}

void nas_acl_action_t::_set_opaque_data (const nas_acl_common_data_list_t& data_list)
{
    constexpr uint_t elem_per_obj = 2;
    uint_t num_objs = data_list.size()/elem_per_obj;

    for (uint_t iter_obj=0; iter_obj<num_objs; iter_obj++) {
        auto elem_num = iter_obj * elem_per_obj;
        _nas2ndi_oid_tbl [data_list.at(elem_num).obj_id] =
                std::move (data_list.at (elem_num+1).ndi_obj_id_table);
    }
}

void nas_acl_action_t::set_opaque_data_action_val (const nas_acl_common_data_list_t& data_list)
{
    _a_info.values_type = NDI_ACL_ACTION_OBJ_ID;
    _set_opaque_data (data_list);
}

void nas_acl_action_t::set_opaque_data_list_action_val (const nas_acl_common_data_list_t& data_list)
{
    _a_info.values_type = NDI_ACL_ACTION_OBJ_ID_LIST;
    _set_opaque_data (data_list);
}

void nas_acl_action_t::get_opaque_data_action_val (nas_acl_common_data_list_t& data_list) const
{
    for (auto& nas2ndi_oid_pair: _nas2ndi_oid_tbl) {
        nas_acl_common_data_t data0;
        data0.obj_id = nas2ndi_oid_pair.first;
        data_list.push_back (data0);

        nas_acl_common_data_t data1;
        data1.ndi_obj_id_table = nas2ndi_oid_pair.second;
        data_list.push_back (data1);
    }
}

void nas_acl_action_t::set_action_ifindex (const nas_acl_common_data_list_t& data_list)
{
    auto ifindex = data_list.at(0).ifindex;
    _ifindex_list.push_back (ifindex);

    if (nas_acl_utl_is_ifidx_type_lag (ifindex)) {

        nas::ndi_obj_id_table_t tmp_ndi_oid_tbl;
        if (dn_nas_lag_get_ndi_ids (ifindex, &tmp_ndi_oid_tbl) != STD_ERR_OK) {
           throw nas::base_exception {NAS_ACL_E_ATTR_VAL, __PRETTY_FUNCTION__,
                                      "Failed to get LAG NDI IDs"};
        }
        auto oid = static_cast <nas_obj_id_t> (ifindex);
        _nas2ndi_oid_tbl [oid] =    std::move (tmp_ndi_oid_tbl);
        _a_info.values_type    = NDI_ACL_ACTION_OBJ_ID;

    } else {
        // Convert to NPU and port
        interface_ctrl_t  intf_ctrl {};
        nas_acl_utl_ifidx_to_ndi_port (ifindex, &intf_ctrl);
        _a_info.values_type              = NDI_ACL_ACTION_PORT;

        _a_info.values.ndi_port.npu_id   = intf_ctrl.npu_id;
        _a_info.values.ndi_port.npu_port = intf_ctrl.port_id;
    }
}

void nas_acl_action_t::get_action_ifindex (nas_acl_common_data_list_t& data_list) const
{
    nas_acl_common_data_t data;

    if (!_ifindex_list.empty ()) {
        data.ifindex =_ifindex_list.at (0);
        data_list.push_back (data);
    }
}

void nas_acl_action_t::set_action_ifindex_list (const nas_acl_common_data_list_t& val_list)
{
    _a_info.values_type  = NDI_ACL_ACTION_PORTLIST;

    for (auto match_data: val_list) {
        for (auto port: match_data.ifindex_list) {
            if (nas_acl_utl_is_ifidx_type_lag(port)) {
                NAS_ACL_LOG_ERR("LAG port %d is not allowed to be added to port list", port);
                continue;
            }
            _ifindex_list.push_back (port);
        }
    }
}

void nas_acl_action_t::get_action_ifindex_list (nas_acl_common_data_list_t& val_list) const
{
    nas_acl_common_data_t if_list_data;

    if (_ifindex_list.size () != 0) {

        for (auto ifindex: _ifindex_list) {
            if_list_data.ifindex_list.push_back (ifindex);
        }

        val_list.push_back (if_list_data);
    }
}

void nas_acl_action_t::get_pkt_action_val (nas_acl_common_data_list_t& val_list) const
{
    nas_acl_common_data_t pkt_action;

    pkt_action.u32 = _a_info.pkt_action;

    val_list.push_back (pkt_action);
}

void nas_acl_action_t::set_pkt_action_val (const nas_acl_common_data_list_t& val_list)
{
    _a_info.values_type = NDI_ACL_ACTION_PKT_ACTION;
    _a_info.pkt_action = (BASE_ACL_PACKET_ACTION_TYPE_t) val_list.at(0).u32;

    auto it = _pkt_action_name_map.find (_a_info.pkt_action);
    if (it == _pkt_action_name_map.end()) {
        throw nas::base_exception {NAS_ACL_E_ATTR_VAL, __PRETTY_FUNCTION__,
            std::string {"Invalid Pkt action "} + std::to_string (_a_info.pkt_action)};
    }
}

bool nas_acl_action_t::_ndi_copy_one_obj_id (ndi_acl_entry_action_t& ndi_action,
                                             npu_id_t npu_id) const
{
    auto it_nas2ndi_oid = _nas2ndi_oid_tbl.begin();
    if (it_nas2ndi_oid == _nas2ndi_oid_tbl.end()) {return false;}

    auto& ndi_oid_tbl = it_nas2ndi_oid->second;

    auto it_npu2ndi_oid = ndi_oid_tbl.find (npu_id);
    if (it_npu2ndi_oid == ndi_oid_tbl.end ()) { return false; }

    NAS_ACL_LOG_DETAIL ("%s: NPU ID %d, NDI Obj Id %" PRIx64,
                        name(), npu_id, it_npu2ndi_oid->second);
    ndi_action.values.ndi_obj_ref = it_npu2ndi_oid->second;
    return true;
}

bool nas_acl_action_t::_ndi_copy_obj_id_list (ndi_acl_entry_action_t& ndi_action,
                                              npu_id_t npu_id,
                                              nas::mem_alloc_helper_t& mem_trakr) const
{
    bool found = false;
    ndi_action.values.ndi_obj_ref_list.count = _nas2ndi_oid_tbl.size();
    ndi_action.values.ndi_obj_ref_list.list =
            mem_trakr.alloc<ndi_obj_id_t> (_nas2ndi_oid_tbl.size());

    ndi_obj_id_t* oid_list = ndi_action.values.ndi_obj_ref_list.list;
    uint_t count = 0;

    for (auto& nas2ndi_oid_pair: _nas2ndi_oid_tbl) {
        auto& ndi_oid_tbl = nas2ndi_oid_pair.second;
        auto it = ndi_oid_tbl.find (npu_id);

        if (it != ndi_oid_tbl.end ()) {
            found = true;
            NAS_ACL_LOG_DETAIL ("%s: NPU ID %d, NDI Obj Id %" PRIx64,
                    name(), npu_id, it->second);
            oid_list[count++] = it->second;
        }
    }
    return found;
}

bool nas_acl_action_t::copy_action_ndi (ndi_acl_action_list_t& ndi_alist,
                                        npu_id_t npu_id,
                                        nas::mem_alloc_helper_t& mem_trakr) const
{
    // For actions with value_type other than Obj ID
    // the NDI value would be readily available - just copy it.
    ndi_alist.push_back (_a_info);

    // For actions of value_type NDI Obj ID data -
    // NDI value has to be extracted based on the NPU
    //
    // Exception - Counter actions - the NDI Obj ID is stored externally
    // in the Counter object
    if (_a_info.values_type == NDI_ACL_ACTION_OBJ_ID && !is_counter ()) {

        if (!_ndi_copy_one_obj_id (ndi_alist.back(), npu_id)) {
            throw nas::base_exception {NAS_ACL_E_FAIL, __PRETTY_FUNCTION__,
                std::string {name()} + ": Could not find object id for NPU " +
                std::to_string (npu_id)};
        }
    }

    if (_a_info.values_type == NDI_ACL_ACTION_OBJ_ID_LIST) {

        if (!_ndi_copy_obj_id_list (ndi_alist.back(), npu_id, mem_trakr)) {
            throw nas::base_exception {NAS_ACL_E_FAIL, __PRETTY_FUNCTION__,
                std::string {name()} + ": Could not find object id for NPU " +
                std::to_string (npu_id)};
        }
    }

    if (_a_info.values_type == NDI_ACL_ACTION_PORT) {

        auto& ndi_action = ndi_alist.back();
        if (ndi_action.values.ndi_port.npu_id != npu_id) {
            ndi_alist.pop_back();
        }
    }

    if (_a_info.values_type == NDI_ACL_ACTION_PORTLIST) {

        // Assert to ensure that we are not overwriting existing portlist
        STD_ASSERT (_a_info.values.ndi_portlist.port_list == NULL);

        // Build the list of NPU specific ports
        std::vector<ndi_port_t> ndi_plist;

        for (auto ifindex: _ifindex_list) {
            // Convert to NPU and port
            interface_ctrl_t  intf_ctrl {};
            nas_acl_utl_ifidx_to_ndi_port (ifindex, &intf_ctrl);
            if (intf_ctrl.npu_id != npu_id) {
                continue;
            }
            ndi_plist.push_back (ndi_port_t {intf_ctrl.npu_id, intf_ctrl.port_id});
        }

        if (ndi_plist.empty()) {
            NAS_ACL_LOG_DETAIL("%s: port list is empty, ignore this action", name());
            ndi_alist.pop_back();
            return true;
        }

        auto& ndi_action = ndi_alist.back();
        ndi_action.values.ndi_portlist.port_count = ndi_plist.size();
        ndi_port_t* plist =  mem_trakr.alloc<ndi_port_t> (ndi_plist.size());
        ndi_action.values.ndi_portlist.port_list = plist;
        memcpy (plist, ndi_plist.data(), sizeof (ndi_port_t) * ndi_plist.size());
    }

    return true;
}

bool nas_acl_action_t::operator!= (const nas_acl_action_t& rhs) const
{
    if (action_type() != rhs.action_type())
        return true;

    switch (_a_info.values_type)
    {
        case NDI_ACL_ACTION_PORT:
            if (_ifindex_list != rhs._ifindex_list) {
                return true;
            }
            break;

        case NDI_ACL_ACTION_OBJ_ID:
            if (_nas_oid != rhs._nas_oid) {
                return true;
            }
            /* Intentional Fall through */
        case NDI_ACL_ACTION_OBJ_ID_LIST:
            if (_nas2ndi_oid_tbl != rhs._nas2ndi_oid_tbl) {
                NAS_ACL_LOG_DETAIL ("NAS to NDI OID Table modified for action %s",
                                    name());
                return true;
            }
            break;

        case NDI_ACL_ACTION_NO_VALUE:
            break;

        default:
            if (memcmp (&_a_info, &rhs._a_info, sizeof(_a_info))) return true;
            break;
    }

    return false;
}

static const char* _get_pkt_action_name (BASE_ACL_PACKET_ACTION_TYPE_t  type) noexcept
{
    auto it = _pkt_action_name_map.find (type);
    if (it == _pkt_action_name_map.end()) {
        return "Invalid Packet Action Type";
    }
    return it->second;
}

void nas_acl_action_t::dbg_dump () const
{
    NAS_ACL_LOG_DUMP ("Action: %s", name ());

    switch (_a_info.values_type) {

        case NDI_ACL_ACTION_NO_VALUE:
            break;

        case NDI_ACL_ACTION_PKT_ACTION:
            NAS_ACL_LOG_DUMP ("  Pkt action = %s", _get_pkt_action_name(_a_info.pkt_action));
            break;

        case NDI_ACL_ACTION_PORT:
        case NDI_ACL_ACTION_PORTLIST:
            NAS_ACL_LOG_DUMP ("  Port = ");
            for (auto ifindex: _ifindex_list) {
                NAS_ACL_LOG_DUMP ("%d, ", ifindex);
            }
            NAS_ACL_LOG_DUMP ("");
            break;

        case NDI_ACL_ACTION_U32:
            NAS_ACL_LOG_DUMP ("  U32 = %d",_a_info.values.u32);
            break;

        case NDI_ACL_ACTION_U16:
            NAS_ACL_LOG_DUMP ("  U16 = %d",_a_info.values.u16);
            break;

        case NDI_ACL_ACTION_U8:
            NAS_ACL_LOG_DUMP ("   U8 = %d",_a_info.values.u8);
            break;

        case NDI_ACL_ACTION_OBJ_ID:
            if (is_counter ()) {
                NAS_ACL_LOG_DUMP ("     counter id = %ld", counter_id());
                break;
            } else if (action_type() == BASE_ACL_ACTION_TYPE_REDIRECT_PORT) {
                NAS_ACL_LOG_DUMP ("    Port = ");
                for (auto ifindex: _ifindex_list) {
                    NAS_ACL_LOG_DUMP ("%d, ", ifindex);
                }
                NAS_ACL_LOG_DUMP ("");
            } else {
                for (auto nas2ndi_oid_pair: _nas2ndi_oid_tbl) {
                    NAS_ACL_LOG_DUMP ("     nas external obj id ref = %ld",nas2ndi_oid_pair.first);
                }
            }

            for (auto nas2ndi_oid_pair: _nas2ndi_oid_tbl) {
                auto& ndi_oid_tbl = nas2ndi_oid_pair.second;
                NAS_ACL_LOG_DUMP ("     External NDI obj IDs (blob): count = %ld     ",
                                   ndi_oid_tbl.size());
                for (auto& npu2ndi_oid_pair: ndi_oid_tbl) {
                    NAS_ACL_LOG_DUMP ("(NPU %d, %ld) ",
                            npu2ndi_oid_pair.first, npu2ndi_oid_pair.second );
                }
            }
            NAS_ACL_LOG_DUMP ("");
            break;

        case NDI_ACL_ACTION_MAC_ADDR:
            NAS_ACL_LOG_DUMP ("  mac_match = %0x:%0x:%0x:%0x:%0x:%0x ",
                    _a_info.values.mac[0], _a_info.values.mac[1],
                    _a_info.values.mac[2], _a_info.values.mac[3],
                    _a_info.values.mac[4], _a_info.values.mac[5]);
            break;

        case NDI_ACL_ACTION_IPV4_ADDR:
            {
                char buff [INET_ADDRSTRLEN];
                NAS_ACL_LOG_DUMP ("   ipv4 address = %s",
                                  inet_ntop (AF_INET, &_a_info.values.ipv4, buff, sizeof (buff)));
            }
            break;

        case NDI_ACL_ACTION_IPV6_ADDR:
            {
                char buff [INET6_ADDRSTRLEN];
                NAS_ACL_LOG_DUMP ("   ipv6 address = %s",
                                  inet_ntop (AF_INET6, &_a_info.values.ipv6, buff, sizeof (buff)));
            }
            break;

        default:
            break;
    }
}
