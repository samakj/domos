import os


def link_cpp_libs(path: str) -> None:
    os.system(
        f"""
if [ ! -d $DOMOS_ROOT/devices/{path}/lib ];
then
    ln -s $DOMOS_ROOT/shared/cpp $DOMOS_ROOT/devices/{path}/lib;
fi
    """
    )
