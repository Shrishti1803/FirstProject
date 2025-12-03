#ifndef WISHLISTFUNCTIONS_H
#define WISHLISTFUNCTIONS_H

#include <vector>
#include <cppconn/connection.h>
#include "Wishlist.h"

bool addToWishlist(sql::Connection* con, int customerId, int productId);
bool removeFromWishlist(sql::Connection* con, int customerId, int productId);
std::vector<Wishlist> viewWishlist(sql::Connection* con, int customerId);

// Menu
void runWishlistMenu(sql::Connection* con, int customerId);

#endif
