from typing import List, Dict

class Presets:
    """
    Known Signal Presets
    """

    PRESETS = {
    }

    @classmethod
    def load(cls, key: str) -> List[int]:
        """
        load preset by key
        (raise KeyError if not found)

        :param key: the key of preset
        :returns: the preset
        """

        return cls.PRESETS[key]['sequence']

    @classmethod
    def list_all(cls) -> Dict[str, Dict]:
        """
        list all the known presets

        :returns: all the presets
        """

        return cls.PRESETS
