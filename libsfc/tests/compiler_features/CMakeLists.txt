# Compiler feature tests

# # Fold Expression Support
# try_compile(FOLD_EXPRESSION_SUPPORTED
#             ${CMAKE_CURRENT_BINARY_DIR}/FOLD_EXPRESSION_SUPPORTED
#             ${CMAKE_CURRENT_SOURCE_DIR}/FOLD_EXPRESSIONS
#             FOLD_EXPRESSION_SUPPORT)

# BMI2 Support
try_compile(BMI2_SUPPORTED
            ${CMAKE_CURRENT_BINARY_DIR}/BMI2
            ${CMAKE_CURRENT_SOURCE_DIR}/BMI2
            BMI2_SUPPORT)
