# Copyright (C) 2011 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/aosp_base.mk)

# Inherit some common Omni stuff.
$(call inherit-product, vendor/omni/config/common_tablet.mk)

# Inherit from wx_na_wf device
$(call inherit-product, device/nvidia/shieldtablet/device.mk)

# Setup device specific product configuration.
PRODUCT_NAME := omni_shieldtablet
PRODUCT_DEVICE := shieldtablet
PRODUCT_BRAND := nvidia
PRODUCT_MANUFACTURER := nvidia
PRODUCT_MODEL := shieldtablet
