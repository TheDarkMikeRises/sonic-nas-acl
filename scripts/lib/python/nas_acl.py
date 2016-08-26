#!/usr/bin/python
#
# Copyright (c) 2015 Dell Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may
# not use this file except in compliance with the License. You may obtain
# a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
# THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
# LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
# FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
#
# See the Apache Version 2.0 License for specific language governing
# permissions and limitations under the License.
#

"""
The NAS ACL Python module is a user-friendly wrapper over the generic CPS Python
module for configuring the Base ACL.

Advatntages over generic CPS module and its utilities -

 - Knows types for all attributes in the Base ACL Yang model - types need not
   be taught explicitly
 - Knows the value attributes for all the ACL Filter and Action types - allows
   filters and actions to be specified as simple python dict type-value pairs
 - Knows the ACL yang model enum name to number mapping - allows Filter and
   Action names from the yang model to be used directly.
 - Hides CPS object creation/population, attachment to CPS Transaction
   internally behind single function call.

Checkout the nas_acl_simple_example.py for a simple usage
Refer the nas_acl_crud_example.py for a more extensive usage
"""

from nas_acl_table import *
from nas_acl_entry import *
from nas_acl_counter import *
from nas_acl_stats import *
import cps
import cps_utils

def create_table(stage, prio, allow_filters, switch_id=0):
    t = TableCPSObj(stage=stage, priority=prio, switch_id=switch_id)

    for f in allow_filters:
        t.add_allow_filter(f)

    upd = ('create', t.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Table create failed")

    t = TableCPSObj(cps_data=r[0])
    table_id = t.extract_id()
    print "Created Table " + str(table_id)
    return table_id


def create_entry(table_id, prio, filter_map, action_map, switch_id=0):

    e = EntryCPSObj(table_id=table_id, priority=prio, switch_id=switch_id)

    for ftype, fval in filter_map.items():
        e.add_match_filter(filter_type=ftype, filter_val=fval)

    for atype, aval in action_map.items():
        e.add_action(action_type=atype, action_val=aval)

    upd = ('create', e.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Entry create failed")

    e = EntryCPSObj(cps_data=r[0])
    entry_id = e.extract_id()
    print "Created Entry " + str(entry_id)
    return entry_id


def create_counter(table_id, types=['BYTE'], switch_id=0):
    c = CounterCPSObj(table_id=table_id, types=types, switch_id=switch_id)
    upd = ('create', c.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Counter create failed")

    c = CounterCPSObj(cps_data=r[0])
    counter_id = c.extract_id()
    print "Created Counter " + str(counter_id)
    return counter_id

# Add another filter to the ACL entry


def append_entry_filter(table_id, entry_id, filter_type, filter_val):
    e = EntryCPSObj(
        table_id=table_id,
        entry_id=entry_id,
        filter_type=filter_type)
    e.set_filter_val(filter_val)
    upd = ('create', e.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Entry filter append failed")

# Change existing filter value in the ACL entry


def mod_entry_filter(table_id, entry_id, filter_type, filter_val):
    e = EntryCPSObj(
        table_id=table_id,
        entry_id=entry_id,
        filter_type=filter_type)
    e.set_filter_val(filter_val)
    upd = ('set', e.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Entry filter mod failed")


# Remove a filter from the ACL entry
def remove_entry_filter(table_id, entry_id, filter_type):
    e = EntryCPSObj(
        table_id=table_id,
        entry_id=entry_id,
        filter_type=filter_type)
    upd = ('delete', e.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Entry filter remove failed")


# Add another action to the ACL entry
def append_entry_action(table_id, entry_id, action_type, action_val):
    e = EntryCPSObj(
        table_id=table_id,
        entry_id=entry_id,
        action_type=action_type)
    e.set_action_val(action_val)
    upd = ('create', e.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Entry action append failed")


# Change existing action value in the ACL entry
def mod_entry_action(table_id, entry_id, action_type, action_val):
    e = EntryCPSObj(
        table_id=table_id,
        entry_id=entry_id,
        action_type=action_type)
    e.set_action_val(action_val)
    upd = ('set', e.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Entry action mod failed")


# Remove an action from the ACL entry
def remove_entry_action(table_id, entry_id, action_type):
    e = EntryCPSObj(
        table_id=table_id,
        entry_id=entry_id,
        action_type=action_type)
    upd = ('delete', e.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Entry action remove failed")


# Completely overwrite the filter list with another set of filters
def replace_entry_filter_list(table_id, entry_id, filter_map):
    e = EntryCPSObj(table_id=table_id, entry_id=entry_id)

    for ftype, fval in filter_map.items():
        e.add_match_filter(filter_type=ftype, filter_val=fval)

    upd = ('set', e.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Entry filter-list replace failed")

# Completely overwrite the action list with another set of actions


def replace_entry_action_list(table_id, entry_id, action_map):
    e = EntryCPSObj(table_id=table_id, entry_id=entry_id)

    for atype, aval in action_map.items():
        e.add_action(action_type=atype, action_val=aval)

    upd = ('set', e.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Entry action-list replace failed")


def print_table(table_id=None):
    t = TableCPSObj(table_id=table_id)
    r = []
    if not cps.get([t.data()], r):
        print 'CPS Get failed for ACL Table' + str(table_id)
        return
    for t_cps in r:
        t = TableCPSObj(cps_data=t_cps)
        t.print_obj()


def print_entry(table_id=None, entry_id=None):
    e = EntryCPSObj(table_id=table_id, entry_id=entry_id)
    r = []
    if not cps.get([e.data()], r):
        print 'CPS Get failed for ACL Entry' + str(entry_id)
        return
    for e_cps in r:
        e = EntryCPSObj(cps_data=e_cps)
        e.print_obj()


def print_counter(table_id=None, counter_id=None):
    c = CounterCPSObj(table_id=table_id, counter_id=counter_id)
    r = []
    if not cps.get([c.data()], r):
        print 'CPS Get failed for ACL Counter' + str(counter_id)
        return
    for c_cps in r:
        c = CounterCPSObj(cps_data=c_cps)
        c.print_obj()


def print_stats(table_id=None, counter_id=None):
    c = StatsCPSObj(table_id=table_id, counter_id=counter_id)
    r = []
    if not cps.get([c.data()], r):
        print 'CPS Get failed for ACL Counter Stats' + str(counter_id)
        return
    for c_cps in r:
        c = StatsCPSObj(cps_data=c_cps)
        c.print_obj()


# Clean up
def delete_entry(table_id, entry_id):
    e = EntryCPSObj(table_id=table_id, entry_id=entry_id)

    upd = ('delete', e.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Entry delete failed")


def delete_counter(table_id, counter_id):
    c = CounterCPSObj(table_id=table_id, counter_id=counter_id)

    upd = ('delete', c.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Counter delete failed")


def delete_table(table_id):
    t = TableCPSObj(table_id=table_id)

    upd = ('delete', t.data())
    r = cps_utils.CPSTransaction([upd]).commit()

    if r == False:
        raise RuntimeError("Table delete failed")
