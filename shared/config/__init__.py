import os
from pathlib import Path
import yaml
from typing import Any, Optional, Union

from .models.devices import DevicesConfig
from .models.location import LocationConfig
from .models.ntp import NTPConfig
from .models.wifi import WifiConfig


class _Config:
    flattened_secrets: Optional[dict[str, Any]] = None

    _devices: Optional[DevicesConfig] = None
    _location: Optional[LocationConfig] = None
    _ntp: Optional[NTPConfig] = None
    _wifi: Optional[WifiConfig] = None

    @staticmethod
    def flatten_dict(obj: dict[str, Any], prefix: str = "") -> dict[str, Any]:
        prefix = f"{prefix}." if prefix else ""
        flattened: dict[str, Any] = {}

        for key, value in obj.items():
            if isinstance(value, dict):
                flattened = {**flattened, **Config.flatten_dict(value, f"{prefix}{key}")}
            else:
                flattened[f"{prefix}{key}"] = value

        return flattened

    @staticmethod
    def get_file_path(path: str) -> str:
        return os.path.join(os.path.dirname(os.path.realpath(__file__)), path)

    def get_from_file(self, path: str) -> dict[str, Any]:
        if self.flattened_secrets is None:
            self.flattened_secrets = self.get_flattened_secrets()

        with open(Config.get_file_path(path)) as file:
            content = file.read()
            for key, value in self.flattened_secrets.items():
                if isinstance(value, (int, float)):
                    content = content.replace(key, f"{value}")
                else:
                    content = content.replace(key, value)
            return yaml.load(content, Loader=yaml.FullLoader)

    def get_flattened_secrets(self) -> dict[str, Any]:
        with open(Config.get_file_path("secrets.yml")) as file:
            secrets = yaml.load(file, Loader=yaml.FullLoader)
            return Config.flatten_dict(obj=secrets, prefix="secrets")

    def inject_config_into_template(
        self,
        config: dict[str, Any],
        input_path: Union[Path, str],
        output_path: Union[Path, str],
        template_prefix: str = "",
        template_suffix: str = "",
        key_prefix: Optional[str] = None,
    ) -> None:
        output_text = ""

        if not Path(input_path).exists():
            raise ValueError(f"Path does not exist: {input_path}")

        with open(file=input_path, mode="r") as input_file:
            output_text = input_file.read()

        for key, value in Config.flatten_dict(config).items():
            output_text = output_text.replace(
                f"{template_prefix}"
                + f"{f'{key_prefix}.' if key_prefix is not None else ''}{key}"
                + f"{template_suffix}",
                str(value),
            )

        with open(file=output_path, mode="w") as output_file:
            output_file.write(output_text)

    def inject_all_configs_into_template(
        self,
        input_path: Union[Path, str],
        output_path: Union[Path, str],
        template_prefix: str = "",
        template_suffix: str = "",
        extra_config: dict[str, Any] = {},
    ) -> None:
        self.inject_config_into_template(
            config=self.devices.dict(),
            input_path=input_path,
            output_path=output_path,
            template_prefix=template_prefix,
            template_suffix=template_suffix,
            key_prefix="devices",
        )
        self.inject_config_into_template(
            config=self.location.dict(),
            input_path=output_path,
            output_path=output_path,
            template_prefix=template_prefix,
            template_suffix=template_suffix,
            key_prefix="location",
        )
        self.inject_config_into_template(
            config=self.ntp.dict(),
            input_path=output_path,
            output_path=output_path,
            template_prefix=template_prefix,
            template_suffix=template_suffix,
            key_prefix="ntp",
        )
        self.inject_config_into_template(
            config=self.wifi.dict(),
            input_path=output_path,
            output_path=output_path,
            template_prefix=template_prefix,
            template_suffix=template_suffix,
            key_prefix="wifi",
        )
        self.inject_config_into_template(
            config=extra_config,
            input_path=output_path,
            output_path=output_path,
            template_prefix=template_prefix,
            template_suffix=template_suffix,
        )

    @property
    def devices(self) -> DevicesConfig:
        if self._devices is None:
            self._devices = DevicesConfig.parse_obj(self.get_from_file(path="devices.yml"))
        return self._devices

    @property
    def location(self) -> LocationConfig:
        if self._location is None:
            self._location = LocationConfig.parse_obj(self.get_from_file(path="location.yml"))
        return self._location

    @property
    def ntp(self) -> NTPConfig:
        if self._ntp is None:
            self._ntp = NTPConfig.parse_obj(self.get_from_file(path="ntp.yml"))
        return self._ntp

    @property
    def wifi(self) -> WifiConfig:
        if self._wifi is None:
            self._wifi = WifiConfig.parse_obj(self.get_from_file(path="wifi.yml"))
        return self._wifi


Config = _Config()
