#ifndef RBAC_DEFAULT_PERMISSIONS_H
#define RBAC_DEFAULT_PERMISSIONS_H

enum RBACDefaultRankPermissions {
    RBAC_RANK_PERMISSION_ADMIN_PANEL = 1 << 0,
    RBAC_RANK_PERMISSION_USE_REDIRECT = 1 << 1,
};

#endif