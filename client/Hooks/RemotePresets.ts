import { useState, useEffect } from 'react';

import Preset from '#/Model/Preset';
import { provideSingletonApi } from '#/Provider/IrClientProvider';

type Presets = Preset[];
type PresetStateSet = [Presets, React.Dispatch<React.SetStateAction<Presets>>];

const api = provideSingletonApi();

const useRemotePresets = (initialState: Presets = []): PresetStateSet => {
    const [presets, setPresets] = useState<Presets>(initialState);
    useEffect(() => {
        let disposed = false;

        (async () => {
            const newPresets = await api.list();
            if (!disposed) {
                setPresets(newPresets);
            }
        })();

        return () => { disposed = true; };
    }, []);

    return [presets, setPresets];
};

export default useRemotePresets;
