
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
 * \file   nas_acl_switch.h
 * \brief  NAS ACL Switch Object
 */

#ifndef _NAS_ACL_SWITCH_H_
#define _NAS_ACL_SWITCH_H_

#include "nas_base_utils.h"
#include "nas_base_obj.h"
#include "nas_acl_counter.h"
#include "nas_acl_entry.h"
#include "nas_acl_table.h"
#include <map>
#include <unordered_map>

class nas_acl_switch : public nas::base_switch_t
{
    public:
        // Generated ACL Table IDs will wrap around after this max
        static const size_t NAS_ACL_TABLE_ID_MAX = 500;
        // Generated ACL Entry IDs will wrap around after this max
        static const size_t NAS_ACL_ENTRY_ID_MAX  = 4094;

        ///// Typedefs ///////
        typedef std::map<nas_obj_id_t, nas_acl_table> table_list_t;
        typedef table_list_t::iterator table_iter_t;
        typedef table_list_t::const_iterator const_table_iter_t;

        typedef std::map<nas_obj_id_t, nas_acl_entry> entry_list_t;
        typedef entry_list_t::iterator entry_iter_t;
        typedef entry_list_t::const_iterator const_entry_iter_t;

        typedef std::map<nas_obj_id_t, nas_acl_counter_t> counter_list_t;

        ///// Constructor ////
        nas_acl_switch (nas_obj_id_t id): nas::base_switch_t (id) {};

        ///////// Accessors ///////
        // ACL Table Get
        nas_acl_table&        get_table (nas_obj_id_t tbl_id);
        nas_acl_table*        find_table (nas_obj_id_t tbl_id) noexcept;
        const table_list_t&   table_list () const noexcept {return _tables;}

        // ACL Entry Get
        nas_acl_entry&        get_entry (nas_obj_id_t tbl_id,
                                         nas_obj_id_t entry_id);
        nas_acl_entry*        find_entry (nas_obj_id_t tbl_id,
                                          nas_obj_id_t entry_id) noexcept;
        const entry_list_t&   entry_list (nas_obj_id_t tbl_id) const;

        nas_acl_counter_t&    get_counter (nas_obj_id_t tbl_id,
                                           nas_obj_id_t counter_id);
        nas_acl_counter_t*    find_counter (nas_obj_id_t tbl_id,
                                            nas_obj_id_t counter_id) noexcept;
        const counter_list_t& counter_list (nas_obj_id_t tbl_id) const;

        ///////// Modifiers //////////
        ///// ACL Table list
        nas_acl_table& save_table (nas_acl_table&& tbl_temp) noexcept;
        void remove_table (nas_obj_id_t id) noexcept;
        nas_obj_id_t alloc_table_id () {return _tableid_gen.alloc_id ();}
        bool reserve_table_id (nas_obj_id_t id);
        void release_table_id (nas_obj_id_t table_id) noexcept
        { _tableid_gen.release_id (table_id); }

        ///// ACL entry  list
        nas_acl_entry&  save_entry (nas_acl_entry&& entry_temp) noexcept;
        void remove_entry_from_table (nas_obj_id_t table_id,
                                      nas_obj_id_t entry_id) noexcept;
        nas_obj_id_t alloc_entry_id_in_table (nas_obj_id_t table_id);
        bool reserve_entry_id_in_table (nas_obj_id_t table_id, nas_obj_id_t id);
        void release_entry_id_in_table (nas_obj_id_t table_id,
                                        nas_obj_id_t entry_id) noexcept;

        ///// ACL counter  list
        nas_acl_counter_t&  save_counter (nas_acl_counter_t&& counter_temp) noexcept;
        void remove_counter_from_table (nas_obj_id_t table_id,
                                        nas_obj_id_t counter_id) noexcept;
        nas_obj_id_t alloc_counter_id_in_table (nas_obj_id_t table_id);
        bool reserve_counter_id_in_table (nas_obj_id_t table_id, nas_obj_id_t id);
        void release_counter_id_in_table (nas_obj_id_t table_id,
                                          nas_obj_id_t counter_id) noexcept;

    private:

        struct acl_table_container_t
        {
            nas::id_generator_t  _entry_id_gen {NAS_ACL_ENTRY_ID_MAX};
            entry_list_t     _acl_entries;
            nas::id_generator_t  _counter_id_gen {NAS_ACL_ENTRY_ID_MAX};
            counter_list_t     _acl_counters;
        };

        typedef std::unordered_map<nas_obj_id_t, acl_table_container_t>
                table_container_list_t;

        table_list_t            _tables;
        table_container_list_t  _table_containers;

        nas::id_generator_t          _tableid_gen {NAS_ACL_TABLE_ID_MAX};
};

#endif
