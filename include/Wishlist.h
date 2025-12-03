#ifndef WISHLIST_H
#define WISHLIST_H

class Wishlist {
private:
    int wishlist_id;
    int customer_id;
    int product_id;

public:
    Wishlist();
    Wishlist(int w_id, int c_id, int p_id);

    int getWishlistID() const { return wishlist_id; }
    int getCustomerID() const { return customer_id; }
    int getProductID() const { return product_id; }
};

#endif
