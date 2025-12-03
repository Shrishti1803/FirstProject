#include "Wishlist.h"

Wishlist::Wishlist() {
    wishlist_id = 0;
    customer_id = 0;
    product_id = 0;
}

Wishlist::Wishlist(int w_id, int c_id, int p_id) {
    wishlist_id = w_id;
    customer_id = c_id;
    product_id = p_id;
}
