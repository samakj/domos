from pydantic import BaseModel


class LocationConfig(BaseModel):
    latitude: float
    longitude: float
