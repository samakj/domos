import os

from cli import cli


if __name__ == "__main__":
    os.system(
        """
if [ ! -d $DOMOS_ROOT/dx/cli/shared ];
then
    ln -s $DOMOS_ROOT/shared $DOMOS_ROOT/dx/cli/shared;
fi
    """
    )
    cli()
