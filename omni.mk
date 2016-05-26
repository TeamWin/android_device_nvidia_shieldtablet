# Inherit device configuration for shieldtablet.
$(call inherit-product, device/nvidia/shieldtablet/full_shieldtablet.mk)

# Inherit some common OMNI stuff.
$(call inherit-product, vendor/omni/config/common_full_phone.mk)

PRODUCT_NAME := omni_shieldtablet
PRODUCT_DEVICE := shieldtablet
