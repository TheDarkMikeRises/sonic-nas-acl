
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


#include "nas_acl_cps_ut.h"

#define NAS_ACL_UT_BREAK_ON_FAILURE(_rc) if(!rc) break;

TEST (nas_acl_table, create_test)
{
    bool rc;

    rc = nas_acl_ut_table_create ();
    ASSERT_TRUE (rc);

    /* Cleanup */
    nas_acl_ut_table_delete ();
}

TEST (nas_acl_table, get_test)
{
    bool rc;

    rc = nas_acl_ut_table_create ();
    ASSERT_TRUE (rc);

    rc = nas_acl_ut_table_get ();

    /* Cleanup */
    nas_acl_ut_table_delete ();
    ASSERT_TRUE (rc);
}

TEST (nas_acl_table, delete_test)
{
    bool rc;

    rc = nas_acl_ut_table_create ();
    ASSERT_TRUE (rc);

    rc = nas_acl_ut_table_delete ();

    /* Cleanup */
    nas_acl_ut_table_get ();
    ASSERT_TRUE (rc);
}

TEST (nas_acl_entry, create_test)
{
    bool rc;

    rc = nas_acl_ut_table_create ();
    ASSERT_TRUE (rc);

    rc = nas_acl_ut_entry_create_test (g_nas_acl_ut_tables [0]);

    /* Cleanup */
    nas_acl_ut_entry_delete_test (g_nas_acl_ut_tables [0]);
    nas_acl_ut_table_delete ();

    ASSERT_TRUE (rc);
}

TEST (nas_acl_entry, modify_test)
{
    bool rc;

    rc = nas_acl_ut_table_create ();
    ASSERT_TRUE (rc);

    if (!nas_acl_ut_entry_create_test (g_nas_acl_ut_tables [0])) {
        nas_acl_ut_table_delete ();
        ASSERT_TRUE (false);
    }

    rc = nas_acl_ut_entry_modify_test (g_nas_acl_ut_tables [0]);

    /* Clean up */
    nas_acl_ut_entry_delete_test (g_nas_acl_ut_tables [0]);
    nas_acl_ut_table_delete ();

    ASSERT_TRUE (rc);
}

TEST (nas_acl_entry, delete_test)
{
    bool rc;

    rc = nas_acl_ut_table_create ();
    ASSERT_TRUE (rc);

    if (!nas_acl_ut_entry_create_test (g_nas_acl_ut_tables [0])) {
        nas_acl_ut_table_delete ();
        ASSERT_TRUE (false);
    }

    rc = nas_acl_ut_entry_delete_test (g_nas_acl_ut_tables [0]);

    /* Cleanup */
    nas_acl_ut_table_delete ();
    ASSERT_TRUE (rc);
}

TEST (nas_acl_entry_get, get_bulk)
{
    bool rc;

    rc = nas_acl_ut_table_create ();
    ASSERT_TRUE (rc);

    if (!nas_acl_ut_entry_create_test (g_nas_acl_ut_tables [0])) {
        nas_acl_ut_table_delete ();
        ASSERT_TRUE (false);
    }

    do {
        rc = nas_acl_ut_entry_get_by_table_test (g_nas_acl_ut_tables [0]);
        NAS_ACL_UT_BREAK_ON_FAILURE (rc);

        rc = nas_acl_ut_entry_get_by_switch_test (NAS_ACL_UT_DEF_SWITCH_ID);
        NAS_ACL_UT_BREAK_ON_FAILURE (rc);

        rc = nas_acl_ut_entry_get_all_test ();
        NAS_ACL_UT_BREAK_ON_FAILURE (rc);
    } while (0);

    /* Cleanup */
    nas_acl_ut_entry_delete_test (g_nas_acl_ut_tables [0]);
    nas_acl_ut_table_delete ();

    ASSERT_TRUE (rc);

    ut_printf ("********** ACL Entry Get BULK Test PASSED **********\r\n");
}

TEST (nas_acl_entry, stats_test)
{
    bool rc;

    rc = nas_acl_ut_table_create ();
    ASSERT_TRUE (rc);

    if (!nas_acl_ut_entry_create_test (g_nas_acl_ut_tables [0])) {
        nas_acl_ut_table_delete ();
        ASSERT_TRUE (false);
    }

    do {
        rc = nas_acl_ut_entry_count_enable (g_nas_acl_ut_tables [0], true, false);
        NAS_ACL_UT_BREAK_ON_FAILURE (rc);
        rc = nas_acl_ut_stats_get_test (g_nas_acl_ut_tables [0]);
        NAS_ACL_UT_BREAK_ON_FAILURE (rc);
        rc = nas_acl_ut_stats_set_test (g_nas_acl_ut_tables [0]);
        NAS_ACL_UT_BREAK_ON_FAILURE (rc);

    } while (0);

    nas_acl_ut_entry_delete_test (g_nas_acl_ut_tables [0]);
    nas_acl_ut_counter_delete (g_nas_acl_ut_tables [0]);
    nas_acl_ut_table_delete ();

    ASSERT_TRUE (rc);
}

TEST (nas_acl_entry, incr_modify_test)
{
    bool rc;

    rc = nas_acl_ut_table_create ();
    ASSERT_TRUE (rc);

    if (!nas_acl_ut_entry_create_test (g_nas_acl_ut_tables [0])) {
        nas_acl_ut_table_delete ();
        ASSERT_TRUE (false);
    }

    rc = nas_acl_ut_entry_incr_modify_test (g_nas_acl_ut_tables [0]);

    /* Clean up */
    nas_acl_ut_entry_delete_test (g_nas_acl_ut_tables [0], false);
    nas_acl_ut_table_delete ();

    ASSERT_TRUE (rc);
}

int main(int argc, char **argv)
{
    nas_acl_ut_env_init ();
    nas_acl_ut_init_tables ();

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
