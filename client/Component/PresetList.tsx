import React from 'react';

import Preset from '#/Component/Preset';
import useRemotePresets from '#/Hooks/RemotePresets';

/**
 * Preset List View
 */
const component: React.FC = () => {
    const [presets] = useRemotePresets();
    const presetViews = presets.map(preset => (<Preset {...preset} />));

    return (
        <article>
            {presetViews}
        </article>
    );
};

export default component;
