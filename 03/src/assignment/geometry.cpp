void createRect(float xsize, float ysize, Color color, float scale)
{
    curr_poly.clear();
    curr_poly.push_back({-xsize / 2.0f,  ysize / 2.0f});
    curr_poly.push_back({-xsize / 2.0f, -ysize / 2.0f});
    curr_poly.push_back({ xsize / 2.0f, -ysize / 2.0f});
    curr_poly.push_back({ xsize / 2.0f,  ysize / 2.0f});

    for (int i = 0; i< 4; i++)
    {
        curr_poly[i] = curr_poly[i] * scale;
    }

    poly_color.push_back(color);

    poly.push_back(curr_poly);
    curr_poly.clear();
}

void createBody()
{
    Color color = {.5f, .5f, .5f};
    createRect(2.0f, 1.0f, color, 1.0f);

    translations.push_back({0.0f, 0.0f});
    rotations.push_back(0.0f);
    parts.push_back(BodyPart::Single);
}

void createHead()
{
    // neck
    Color color = {.45f, .6f, .45f};
    createRect(.5f, 1.0f, color, 1.0f);

    translations.push_back({0.0f, 2.0f});
    rotations.push_back(0.0f);
    parts.push_back(BodyPart::Start);

    // head
    color = {.45f, .9f, .45f};
    createRect(1.0f, 1.0f, color, 1.2f);

    translations.push_back({0.0f, 1.0f});
    rotations.push_back(0.0f);
    parts.push_back(BodyPart::End);
}

void createArm(int direction)
{
    float dir = direction <= 0 ? -1 : 1;

    // chest
    Color color = {0.3f, 0.5f, 0.5f};
    createRect(1.0f, 1.0f, color, 1.0f);

    translations.push_back({1.0f * dir, 1.0f});
    rotations.push_back(0.0f);

    parts.push_back(BodyPart::Start);

    // arm
    for (int i = 0; i < 3; i++)
    {
        color = {0.3f, 0.5f + .1f * i, .5f + .1f * i};
        createRect(1.0f, 0.5f, color, 1.0f);

        translations.push_back({1.0f * dir, 0.0f});
        rotations.push_back(0.0f);

        if (i < 2) parts.push_back(BodyPart::Inside);
        else parts.push_back(BodyPart::End);
    }
}

void createLeg(int direction)
{
    float dir = direction <= 0 ? -1 : 1;

    // 1st
    Color color = {0.8f, 0.5f, .5f};
    createRect(0.4f, 1.5f, color, 1.0f);

    translations.push_back({.5f * dir, -1.5f});
    rotations.push_back(0.0f);
    parts.push_back(BodyPart::Start);

    // 2nd
    color[0] -= .1f;
    createRect(0.4f, 1.5f, color, 1.0f);

    translations.push_back({0.0f, -1.5f});
    rotations.push_back(0.0f);
    parts.push_back(BodyPart::Inside);

    // 3rd
    color[0] -= .1f;
    createRect(.6f, .5f, color, 1.0f);

    translations.push_back({0.0f, -1.3f});
    rotations.push_back(0.0f);
    parts.push_back(BodyPart::End);
}

bool isStart(BodyPart part)
{
    return part == BodyPart::Single || part == BodyPart::Start;
}

bool isEnd(BodyPart part)
{
    return part == BodyPart::Single || part == BodyPart::End;
}
