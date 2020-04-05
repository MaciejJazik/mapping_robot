file(REMOVE_RECURSE
  "../msg_gen"
  "../msg_gen"
  "../src/k2_client-1.0.0/msg"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/download_extra_data.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
